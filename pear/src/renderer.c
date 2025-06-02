#include <renderer.h>
#include <model.h>
#include <alloc.h>
#include <log.h>

#define SOKOL_IMPL
#define SOKOL_GLCORE
#include <sokol_gfx.h>
#include <sokol_log.h>

#include <shaders/shader.h>

typedef struct renderer_t {
    model_t* model;
    sg_shader shader;
    sg_pipeline pipeline;
} renderer_t;

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

    self->model = model_load("assets/Avocado.glb");

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

    return self;
}

void renderer_delete(renderer_t* self) {
    sg_shutdown();
    pear_free(self);
}

void renderer_render(renderer_t *self) {
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    mat4 view_proj = GLM_MAT4_IDENTITY_INIT;
    glm_perspective(glm_rad(45.0f), 800.0f / 600.0f, 0.1f, 100.0f, projection);
    glm_lookat((vec3){ 0.0f, 1.5f, 6.0f }, (vec3){ 0.0f, 0.0f, 0.0f }, (vec3){ 0.0f, 1.0f, 0.0f }, view);
    glm_mat4_mul(projection, view, view_proj);

    static float rotation = 0.0f;
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_scale(model, (vec3){ 50.0f, 50.0f, 50.0f });
    glm_rotate(model, rotation, (vec3){ 0.0f, 1.0f, 0.0f });
    rotation += 0.01f;

    mat4 mvp = GLM_MAT4_IDENTITY_INIT;
    glm_mat4_mul(view_proj, model, mvp);

    vs_uniforms_t uniforms;
    glm_mat4_copy(mvp, (vec4*)uniforms.u_mvp);

    sg_begin_pass(&(sg_pass){
        .swapchain = {
            .width = 800,
            .height = 600,
            .sample_count = 1,
            .color_format = SG_PIXELFORMAT_RGBA8,
            .depth_format = SG_PIXELFORMAT_DEPTH_STENCIL,
            .gl = { .framebuffer = 0 }
        }
    });
    sg_apply_pipeline(self->pipeline);
    sg_apply_uniforms(UB_vs_uniforms, &SG_RANGE(uniforms));

    u32 i;
    mesh_t mesh;
    ARRAY_FOREACH(self->model->meshes, mesh, i) {
        material_t material = material_array_get(self->model->materials, mesh.material_index);

        sg_apply_bindings(&(sg_bindings){
            .vertex_buffers[0] = mesh.vertex_buffer,
            .index_buffer = mesh.index_buffer,
            .images[IMG_u_texture] = material.diffuse_texture.image,
            .samplers[SMP_u_sampler] = material.diffuse_texture.sampler
        });
        sg_draw(0, mesh.num_indices, 1);
    }

    sg_end_pass();
    sg_commit();
}
