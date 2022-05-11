#ifndef GAME_H
#define GAME_H

typedef struct game_data_t
{
    gs_handle(gs_graphics_vertex_buffer_t) vbo;
    gs_handle(gs_graphics_index_buffer_t) ibo;
    gs_handle(gs_graphics_pipeline_t) pip;
    gs_handle(gs_graphics_shader_t) shader;
    gs_handle(gs_graphics_uniform_t) u_projection;
    gs_handle(gs_graphics_uniform_t) u_tex;
    gs_handle(gs_graphics_texture_t) tex;
    f32 vertices[16];
} game_data_t;

// Data //

float v_data[] = {
    // Positions  UVs
    -0.5f, -0.5f,  0.0f, 0.0f,  // Top Left
     0.5f, -0.5f,  1.0f, 0.0f,  // Top Right 
    -0.5f,  0.5f,  0.0f, 1.0f,  // Bottom Left
     0.5f,  0.5f,  1.0f, 1.0f   // Bottom Right
};

uint32_t i_data[] = {
    0, 3, 2,    // First Triangle
    0, 1, 3     // Second Triangle
};

// Shaders
#ifdef GS_PLATFORM_WEB
    #define GS_VERSION_STR "#version 300 es\n"
#else
    #define GS_VERSION_STR "#version 330 core\n"
#endif

// Shaders
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

#endif
