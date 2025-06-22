#include "renderer.h"
#include "cglm/affine.h"
#include "cglm/vec3.h"
#include "event.h"
#include "array.h"
#include "node.h"
#include "nodes/model.h"
#include "nodes/mesh.h"
#include "nodes/pos.h"
#include "nodes/rotation.h"
#include "nodes/scale.h"
#include "nodes/camera.h"
#include "alloc.h"
#include "log.h"
#include <math.h>

#define SOKOL_IMPL
#define SOKOL_GLCORE
#include <sokol_gfx.h>
#include <sokol_log.h>

#include <shaders/shader.h>

typedef struct renderer_t {
    sg_shader shader;
    sg_pipeline pipeline;
    sg_swapchain swapchain;
    model_t* current_model;

    mat4 projection;
    mat4 view;
} renderer_t;

static void renderer_build_view_proj(renderer_t* self, u32 width, u32 height) {
    glm_perspective(glm_rad(45.0f), (f32)width / (f32)height, 0.1f, 100.0f, self->projection);
    glm_lookat((vec3){ 0.0f, 0.0f, 0.0f }, (vec3){ 0.0f, 0.0f, 1.0f }, (vec3){ 0.0f, 1.0f, 0.0f }, self->view);
}

static void renderer_on_event(event_type_t type, void* event, void* user_data) {
    renderer_t* self = (renderer_t*)user_data;

    if (type == EVENT_TYPE_WINDOW_RESIZED) {
        window_resized_event_t* e = (window_resized_event_t*)event;
        renderer_build_view_proj(self, e->width, e->height);
        self->swapchain.width = e->width;
        self->swapchain.height = e->height;
    }
}

renderer_t* renderer_new(void) {
    sg_setup(&(sg_desc){
        .environment = {
             .defaults = {
                .color_format = SG_PIXELFORMAT_RGBA8,
                .depth_format = SG_PIXELFORMAT_DEPTH_STENCIL,
                .sample_count = 1
             }
        },
        .logger.func = slog_func
    });
    PEAR_ASSERT(sg_isvalid(), "failed to initialize sokol!");

    renderer_t* self = PEAR_ALLOC(renderer_t);
    renderer_build_view_proj(self, 800, 600);
    self->swapchain = (sg_swapchain){
        .width = 800,
        .height = 600,
        .sample_count = 1,
        .color_format = SG_PIXELFORMAT_RGBA8,
        .depth_format = SG_PIXELFORMAT_DEPTH_STENCIL,
        .gl = { .framebuffer = 0 }
    };

    self->shader = sg_make_shader(shader_shader_desc(sg_query_backend()));

    self->pipeline = sg_make_pipeline(&(sg_pipeline_desc){
        .shader = self->shader,
        .index_type = SG_INDEXTYPE_UINT32,
        .cull_mode = SG_CULLMODE_FRONT,
        .depth = {
            .compare = SG_COMPAREFUNC_LESS_EQUAL,
            .write_enabled = true
        },
        .layout = {
            .attrs = {
                [ATTR_shader_a_pos].format = SG_VERTEXFORMAT_FLOAT3,
                [ATTR_shader_a_tex_coords].format = SG_VERTEXFORMAT_FLOAT2,
                [ATTR_shader_a_normal].format = SG_VERTEXFORMAT_FLOAT3
            }
        }
    });

    event_subscribe(renderer_on_event, self);

    return self;
}

void renderer_delete(renderer_t* self) {
    sg_shutdown();
    pear_free(self);
}

void renderer_render_mesh(renderer_t* self, mesh_t* mesh, vec3 translation, vec3 rotation, vec3 scale) {
    material_t material = material_array_get(self->current_model->materials, mesh->material_index);

    mat4 transform;
    glm_translate(transform, translation);
    glm_euler(rotation, transform);
    glm_scale(transform, scale);

    vs_uniforms_t uniforms;
    glm_mat4_copy(self->projection, (vec4*)uniforms.u_projection);
    glm_mat4_copy(self->view, (vec4*)uniforms.u_view);
    glm_mat4_copy(transform, (vec4*)uniforms.u_model);
    sg_apply_uniforms(UB_vs_uniforms, &SG_RANGE(uniforms));

    sg_apply_bindings(&(sg_bindings){
        .vertex_buffers[0] = mesh->vertex_buffer,
        .index_buffer = mesh->index_buffer,
        .images[IMG_u_texture] = material.diffuse_texture.image,
        .samplers[SMP_u_sampler] = material.diffuse_texture.sampler
    });
    sg_draw(0, mesh->num_indices, 1);
}

void renderer_handle_camera(renderer_t* self, vec3 translation, vec3 rotation) {
    vec3 world_up = { 0.0f, 1.0f, 0.0f };
    vec3 front;
    vec3 right;
    vec3 up;

    front[0] = cos(glm_rad(rotation[1])) * cos(glm_rad(rotation[0]));
    front[1] = sin(glm_rad(rotation[0]));
    front[2] = sin(glm_rad(rotation[1])) * cos(glm_rad(rotation[0]));
    glm_normalize(front);

    glm_cross(front, world_up, right);
    glm_normalize(right);

    glm_cross(right, front, up);
    glm_normalize(up);

    vec3 target;
    glm_vec3_add(translation, front, target);
    glm_lookat(translation, target, up, self->view);
}

void renderer_handle_node(renderer_t* self, node_t* node, vec3 translation, vec3 rotation, vec3 scale) {
    switch (node->type) {
    case NODE_TYPE_POS: {
        pos_t* pos = (pos_t*)node;
        glm_vec3_add(pos->pos, translation, translation);
        break;
    }
    case NODE_TYPE_ROTATION: {
        rotation_t* r = (rotation_t*)node;
        glm_vec3_add(r->rotation, rotation, rotation);
        break;
    }
    case NODE_TYPE_SCALE: {
        scale_t* s = (scale_t*)node;
        glm_vec3_mul(s->scale, scale, scale);
        break;
    }
    case NODE_TYPE_MODEL:
        self->current_model = (model_t*)node;
        break;
    case NODE_TYPE_MESH:
        renderer_render_mesh(self, (mesh_t*)node, translation, rotation, scale);
        break;
    case NODE_TYPE_CAMERA:
        renderer_handle_camera(self, translation, rotation);
        break;
    default:
        break;
    }

    vec3 local_translation;
    vec3 local_rotation;
    vec3 local_scale;
    glm_vec3_copy(translation, local_translation);
    glm_vec3_copy(rotation, local_rotation);
    glm_vec3_copy(scale, local_scale);

    node_t* child;
    u32 i;
    ARRAY_FOREACH(node->children, child, i) {
        renderer_handle_node(self, child, local_translation, local_rotation, local_scale);
    }
}

void renderer_render_node(renderer_t* self, node_t* node) {
    self->current_model = NULL;

    sg_begin_pass(&(sg_pass){
        .swapchain = self->swapchain
    });
    sg_apply_pipeline(self->pipeline);

    mat4 transform = GLM_MAT4_IDENTITY_INIT;
    renderer_handle_node(self, node, GLM_VEC3_ZERO, GLM_VEC3_ZERO, GLM_VEC3_ONE);

    sg_end_pass();
    sg_commit();
}
