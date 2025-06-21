#include "nodes/model.h"
#include "nodes/mesh.h"
#include "log.h"
#include "sokol_gfx.h"
#include "cgltf.h"
#include "stb_image.h"

sg_buffer model_load_indices(cgltf_primitive primitive, u32* num_indices) {
    *num_indices = cgltf_accessor_unpack_indices(primitive.indices, NULL, 0, 0);
    u32* indices = PEAR_ALLOC_N(u32, *num_indices);
    u32 ret = cgltf_accessor_unpack_indices(primitive.indices, indices, sizeof(u32), *num_indices);
    PEAR_ASSERT(ret != 0, "failed to unpack indices!");

    sg_buffer index_buffer = sg_make_buffer(&(sg_buffer_desc){
        .usage.index_buffer = true,
        .data = {
            .ptr = indices,
            .size = *num_indices * sizeof(u32)
        }
    });
    pear_free(indices);

    return index_buffer;
}

void model_build_buffer(f32** dest, f32* positions, f32* tex_coords, f32* normals, u32 positions_size, u32 tex_coords_size, u32 normals_size) {
    u32 stride = 3 + 2 + 3;
    u32 num_data = positions_size + tex_coords_size + normals_size;
    f32* buffer = PEAR_ALLOC_N(f32, num_data);

    u32 positions_index = 0;
    u32 tex_coords_index = 0;
    u32 normals_index = 0;

    for (u32 i = 0; i < num_data; i += stride) {
        if (positions_index < positions_size) {
            buffer[i] = positions[positions_index++];
            buffer[i + 1] = positions[positions_index++];
            buffer[i + 2] = positions[positions_index++];
        }

        if (tex_coords_index < tex_coords_size) {
            buffer[i + 3] = tex_coords[tex_coords_index++];
            buffer[i + 4] = tex_coords[tex_coords_index++];
        }

        if (normals_index < normals_size) {
            buffer[i + 5] = normals[normals_index++];
            buffer[i + 6] = normals[normals_index++];
            buffer[i + 7] = normals[normals_index++];
        }
    }

    *dest = buffer;
}

u32 model_load_positions(f32** positions, cgltf_attribute attribute, mat4 matrix) {
    u32 num_data = cgltf_accessor_unpack_floats(attribute.data, NULL, 0);
    f32* buffer = PEAR_ALLOC_N(f32, num_data);
    u32 ret = cgltf_accessor_unpack_floats(attribute.data, buffer, num_data);
    PEAR_ASSERT(ret != 0, "failed to unpack position data!");

    for (u32 i = 0; i < num_data; i += 3) {
        glm_mat4_mulv3(matrix, &buffer[i], 1.0f, &buffer[i]);
    }

    *positions = buffer;

    return num_data;
}

u32 model_load_tex_coords(f32** tex_coords, cgltf_attribute attribute) {
    u32 num_data = cgltf_accessor_unpack_floats(attribute.data, NULL, 0);
    f32* buffer = PEAR_ALLOC_N(f32, num_data);
    u32 ret = cgltf_accessor_unpack_floats(attribute.data, buffer, num_data);
    PEAR_ASSERT(ret != 0, "failed to unpack tex coords data!");

    *tex_coords = buffer;

    return num_data;
}

u32 model_load_normals(f32** normals, cgltf_attribute attribute) {
    u32 num_data = cgltf_accessor_unpack_floats(attribute.data, NULL, 0);
    f32* buffer = PEAR_ALLOC_N(f32, num_data);
    u32 ret = cgltf_accessor_unpack_floats(attribute.data, buffer, num_data);
    PEAR_ASSERT(ret != 0, "failed to unpack normal data!");

    *normals = buffer;

    return num_data;
}

sg_buffer model_load_attributes(cgltf_primitive primitive, mat4 matrix) {
    f32* positions = NULL;
    f32* tex_coords = NULL;
    f32* normals = NULL;
    u32 positions_size = 0;
    u32 tex_coords_size = 0;
    u32 normals_size = 0;

    for (u32 i = 0; i < primitive.attributes_count; i++) {
        switch (primitive.attributes[i].type) {
        case cgltf_attribute_type_position:
            positions_size = model_load_positions(&positions, primitive.attributes[i], matrix);
            break;
        case cgltf_attribute_type_texcoord:
            tex_coords_size = model_load_tex_coords(&tex_coords, primitive.attributes[i]);
            break;
        case cgltf_attribute_type_normal:
            normals_size = model_load_normals(&normals, primitive.attributes[i]);
            break;
        default:
            PEAR_WARN("unsupported attribute type %d!", primitive.attributes[i].type);
            break;
        }
    }

    f32* buffer = NULL;
    model_build_buffer(&buffer, positions, tex_coords, normals, positions_size, tex_coords_size, normals_size);

    u32 num_data = positions_size + tex_coords_size + normals_size;
    sg_buffer vertex_buffer = sg_make_buffer(&(sg_buffer_desc){
        .data = {
            .ptr = buffer,
            .size = num_data * sizeof(f32)
        }
    });

    pear_free(positions);
    pear_free(tex_coords);
    pear_free(normals);

    return vertex_buffer;
}

material_t model_create_material(model_t* self, cgltf_material* gltf_material) {
    material_t material;
    strncpy(material.name, gltf_material->name, MATERIAL_NAME_LENGTH);

    cgltf_texture* diffuse_texture = gltf_material->pbr_metallic_roughness.base_color_texture.texture; 
    if (diffuse_texture != NULL) {
        i32 width, height, num_channels;
        u8* image = NULL;

        if (diffuse_texture->image->buffer_view != NULL) {
            const u8* image_data = cgltf_buffer_view_data(diffuse_texture->image->buffer_view);
            image = stbi_load_from_memory(image_data, diffuse_texture->image->buffer_view->size, &width, &height, &num_channels, 4);
        }
        else {
            char texture_path[256];
            sprintf(texture_path, "%s/%s", self->directory, diffuse_texture->image->uri);
            image = stbi_load(texture_path, &width, &height, &num_channels, 4);
        }
        PEAR_ASSERT(image != NULL, "failed to load texture %s!", diffuse_texture->name);

        material.diffuse_texture.image = sg_make_image(&(sg_image_desc){
            .width = width,
            .height = height,
            .pixel_format = SG_PIXELFORMAT_RGBA8,
            .data.subimage[0][0] = {
                .ptr = image,
                .size = (size_t)(width * height * 4) * sizeof(u8)
            }
        });

        cgltf_filter_type mag_filter, min_filter;
        if (diffuse_texture->sampler) {
            mag_filter = diffuse_texture->sampler->mag_filter;
            min_filter = diffuse_texture->sampler->min_filter;
        }
        else {
            mag_filter = cgltf_filter_type_nearest;
            min_filter = cgltf_filter_type_nearest;
        }

        material.diffuse_texture.sampler = sg_make_sampler(&(sg_sampler_desc){
            .mag_filter = mag_filter == cgltf_filter_type_nearest ? SG_FILTER_NEAREST : SG_FILTER_LINEAR,
            .min_filter = min_filter == cgltf_filter_type_nearest ? SG_FILTER_NEAREST : SG_FILTER_LINEAR,
        });
    }

    return material;
}

u32 model_load_material(model_t* self, cgltf_primitive primitive) {
    cgltf_material* gltf_material = primitive.material;

    u32 i;
    material_t material;
    ARRAY_FOREACH(self->materials, material, i) {
        if (strcmp(material.name, gltf_material->name) == 0) {
            return i;
        }
    }

    u32 material_index = material_index = self->materials->length;
    material_array_add(self->materials, model_create_material(self, gltf_material));
    return material_index;
}

void model_handle_mesh(model_t* self, cgltf_mesh* gltf_mesh, mat4 matrix) {
    sg_buffer vertex_buffer;
    sg_buffer index_buffer;
    u32 num_indices;
    u32 material_index;

    for (u32 i = 0; i < gltf_mesh->primitives_count; i++) {
        cgltf_primitive primitive = gltf_mesh->primitives[i];
        PEAR_ASSERT(primitive.type == cgltf_primitive_type_triangles, "unsupported primitive type %d!", primitive.type);

        index_buffer = model_load_indices(primitive, &num_indices);
        vertex_buffer = model_load_attributes(primitive, matrix);
        material_index = model_load_material(self, primitive);
    }

    node_child((node_t*)self, mesh_t, gltf_mesh->name, vertex_buffer, index_buffer, num_indices, material_index);
}

void model_handle_node(model_t* self, cgltf_node* node) {
    mat4 matrix;
    cgltf_float cgltf_matrix[16];
    cgltf_node_transform_world(node, cgltf_matrix);
    glm_mat4_make(cgltf_matrix, matrix);

    if (node->mesh != NULL) {
        model_handle_mesh(self, node->mesh, matrix);
    }
}

NODE_NEW_FUNC_SIGNATURE(model_t, const char* filename) {
    model_t* self = node_new(model_t, parent, NODE_TYPE_MODEL, name);
    self->materials = material_array_new();

    // load the model with cgltf
    cgltf_options options = { 0 };
    cgltf_data* data = NULL;

    cgltf_result result = cgltf_parse_file(&options, filename, &data);
    PEAR_ASSERT(result == cgltf_result_success, "failed to load %s!", filename);

    result = cgltf_load_buffers(&options, data, filename);
    PEAR_ASSERT(result == cgltf_result_success, "failed to load buffers for %s!", filename);

    // find the directory of the model file (useful when loading textures if they are not embedded in the glb)
    char* last_slash = strrchr(filename, '/');
    if (last_slash == NULL) {
        self->directory[0] = '\0';
    }
    else {
        u32 directory_index = last_slash - filename;
        // FIXME: if the copy fails because n > MODEL_DIRECTORY_LENGTH, the directory will be wrong.
        strncpy(self->directory, filename, MODEL_DIRECTORY_LENGTH);
        self->directory[directory_index] = '\0';
    }

    // begin creating the meshes
    for (u32 i = 0; i < data->scenes_count; i++) {
        cgltf_scene scene = data->scenes[i];
        for (u32 j = 0; j < scene.nodes_count; j++) {
            model_handle_node(self, &data->nodes[j]);
        }
    }

    cgltf_free(data);

    return self;
}
