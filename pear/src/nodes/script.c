#include "nodes/script.h"

NODE_NEW_FUNC_SIGNATURE(script_t, script_update_func_t update) {
    script_t* self = node_new(script_t, parent, NODE_TYPE_SCRIPT, name);
    self->update = update;
    return self;
}
