#ifndef NODE_MODEL_H_
#define NODE_MODEL_H_

#include "node.h"
#include "sokol_gfx.h"

#define MATERIAL_NAME_LENGTH 256
#define MODEL_DIRECTORY_LENGTH 256

typedef struct texture_t {
    sg_image image;
    sg_sampler sampler;
} texture_t;

typedef struct material_t {
    char name[MATERIAL_NAME_LENGTH];
    texture_t diffuse_texture;
} material_t;
ARRAY_DECL(material_t, material);

NODE_TYPE_DECL(
    model_t,
    char directory[MODEL_DIRECTORY_LENGTH];
    material_array_t* materials;
);

NODE_NEW_FUNC_SIGNATURE(model_t, const char* filename);



#endif
