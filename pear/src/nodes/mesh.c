#include "nodes/mesh.h"

NODE_NEW_FUNC_SIGNATURE(mesh_t, sg_buffer vertex_buffer, sg_buffer index_buffer, u32 num_indices, u32 material_index) {
    mesh_t* self = node_new(mesh_t, parent, NODE_TYPE_MESH, name);
    self->vertex_buffer = vertex_buffer;
    self->index_buffer = index_buffer;
    self->num_indices = num_indices;
    self->material_index = material_index;
    return self;
}
