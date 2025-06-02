#ifndef MODEL_H_
#define MODEL_H_

#include <array.h>
#include <types.h>
#include <sokol_gfx.h>

#define MODEL_DIRECTORY_LENGTH 256
#define MESH_NAME_LENGTH 256
#define MATERIAL_NAME_LENGTH 256

typedef struct texture_t {
    sg_image image;
    sg_sampler sampler;
} texture_t;

typedef struct mesh_t {
    char name[MESH_NAME_LENGTH];
    sg_buffer vertex_buffer;
    sg_buffer index_buffer;
    u32 num_indices;
    u32 material_index;
} mesh_t;
ARRAY_DECL(mesh_t, mesh);

typedef struct material_t {
    char name[MATERIAL_NAME_LENGTH];
    texture_t diffuse_texture;
} material_t;
ARRAY_DECL(material_t, material);

typedef struct model_t {
    char directory[MODEL_DIRECTORY_LENGTH];
    mesh_array_t* meshes;
    material_array_t* materials;
} model_t;

model_t* model_load(const char* filename);
void model_delete(model_t* self);

#endif
