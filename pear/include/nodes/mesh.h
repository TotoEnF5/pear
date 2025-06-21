#ifndef NODE_MESH_H_
#define NODE_MESH_H_

#include "node.h"
#include "sokol_gfx.h"

NODE_TYPE_DECL(
    mesh_t,
    sg_buffer vertex_buffer;
    sg_buffer index_buffer;
    u32 num_indices;
    u32 material_index;
);

NODE_NEW_FUNC_SIGNATURE(mesh_t, sg_buffer vertex_buffer, sg_buffer index_buffer, u32 num_indices, u32 material_index);

#endif
