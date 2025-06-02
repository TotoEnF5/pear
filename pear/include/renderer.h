#ifndef RENDERER_H_
#define RENDERER_H_

typedef struct renderer_t renderer_t;

renderer_t* renderer_new(void);
void renderer_delete(renderer_t* self);

void renderer_render(renderer_t* self);

#endif
