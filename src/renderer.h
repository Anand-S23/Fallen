#ifndef RENDERER_H
#define RENDERER_H

#include <gunslinger/gs.h>

#define MAX_RENDER_OBJECTS 1000

// Shaders

// TODO: Move shaders out into own files
#ifdef GS_PLATFORM_WEB
    #define GS_VERSION_STR "#version 300 es\n"
#else
    #define GS_VERSION_STR "#version 330 core\n"
#endif

static const char* v_src = GS_VERSION_STR
    "layout(location = 0) in vec2 a_pos;\n"
    "layout(location = 1) in vec2 a_uv;\n"

    "uniform mat4 u_projection;\n"

    "precision mediump float;\n"
    "out vec2 uv;\n"

    "void main()\n"
    "{\n"
        "gl_Position = u_projection * vec4(a_pos, 0.0, 1.0);\n"
        "uv = a_uv;\n"
    "}\0";

static const char* f_src = GS_VERSION_STR
    "precision mediump float;\n"
    "in vec2 uv;\n"
    "out vec4 frag_color;\n"

    "uniform sampler2D u_tex;"

    "void main()\n"
    "{\n"
        "frag_color = texture(u_tex, uv);\n"
    "}\0";


// TODO: See if this is needed
typedef enum rqi_type_t
{
    RQI_TYPE_texture,
    RQI_TYPE_MAX
} rqi_type_t;

typedef struct render_queue_item_t
{
    u32 offset;
    u8 data_count;
    gs_handle(gs_graphics_texture_t) texture;
} render_queue_item_t;

typedef struct render_queue_t
{
    u32 count;
    u32 buffer_used;
    u32 buffer_size;
    void *buffer;
    render_queue_item_t items[MAX_RENDER_OBJECTS];
} render_queue_t;

typedef struct renderer_t
{
    gs_command_buffer_t command_buffer;
    render_queue_t render_queue;
    gs_mat4 projection;
    f32 viewport_width;
    f32 viewport_height;
    gs_graphics_clear_desc_t clear;

    gs_handle(gs_graphics_vertex_buffer_t) texture_vbo;
    gs_handle(gs_graphics_index_buffer_t) texture_ibo;
    gs_handle(gs_graphics_pipeline_t) texture_pipeline;
    gs_handle(gs_graphics_shader_t) texture_shader;

    gs_handle(gs_graphics_uniform_t) u_projection;
    gs_handle(gs_graphics_uniform_t) u_tex;

    void (*set_viewport)(struct renderer_t *renderer, f32 vw, f32 vh);
    void (*set_clear_color)(struct renderer_t *renderer, f32 r, f32 g, f32 b, f32 a);
    void (*draw_texture)(render_queue_t *rq, gs_vec2 pos, gs_vec2 size,
                         gs_handle(gs_graphics_texture_t) texture);
    void (*test_f)(struct renderer_t *renderer);
} renderer_t;

#endif
