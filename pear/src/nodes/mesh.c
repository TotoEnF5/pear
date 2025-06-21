#include "nodes/mesh.h"

void mesh_delete(node_t* s) {
    mesh_t* self = (mesh_t*)s;
    sg_destroy_buffer(self->vertex_buffer);
    sg_destroy_buffer(self->index_buffer);
}

NODE_NEW_FUNC_SIGNATURE(mesh_t, sg_buffer vertex_buffer, sg_buffer index_buffer, u32 num_indices, u32 material_index) {
    mesh_t* self = node_new(mesh_t, parent, NODE_TYPE_MESH, name);
    self->delete_func = mesh_delete;
    self->vertex_buffer = vertex_buffer;
    self->index_buffer = index_buffer;
    self->num_indices = num_indices;
    self->material_index = material_index;
    return self;
}
