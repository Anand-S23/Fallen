#ifndef RENDERER_H
#define RENDERER_H

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

#define TEXTURE_DATA_COUNT 16

typedef struct renderer_t
{
    gs_command_buffer_t command_buffer;
    gs_mat4 projection;
    f32 viewport_width;
    f32 viewport_height;
    f32 clear_color[4];

    gs_handle(gs_graphics_vertex_buffer_t) texture_vbo;
    gs_handle(gs_graphics_index_buffer_t) texture_ibo;
    gs_handle(gs_graphics_pipeline_t) texture_pipeline;
    gs_handle(gs_graphics_shader_t) texture_shader;

    gs_handle(gs_graphics_uniform_t) u_projection;
    gs_handle(gs_graphics_uniform_t) u_tex;
} renderer_t;

#endif
