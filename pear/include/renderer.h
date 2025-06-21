#ifndef RENDERER_H_
#define RENDERER_H_

#include "node.h"

typedef struct renderer_t renderer_t;

renderer_t* renderer_new(void);
void renderer_delete(renderer_t* self);

void renderer_render_node(renderer_t* self, node_t* node);

#endif
