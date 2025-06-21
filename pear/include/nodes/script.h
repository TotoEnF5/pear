#ifndef NODE_SCRIPT_H_
#define NODE_SCRIPT_H_

#include "node.h"

typedef struct script_t script_t;
typedef void(*script_update_func_t)(script_t* self, f64 dt);

NODE_TYPE_DECL(
    script_t,
    script_update_func_t update;
);

NODE_NEW_FUNC_SIGNATURE(script_t, script_update_func_t update);

#endif
