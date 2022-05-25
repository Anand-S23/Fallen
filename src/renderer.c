#include "renderer.h"

static void test_func(renderer_t *renderer)
{
    printf("test_func was called\n");
}

// Util // 

static void append_data_to_buffer(render_queue_t *rq, void *data, size_t size)
{
    memcpy(rq->buffer + rq->buffer_used, data, size);
    rq->buffer_used += size;
}

// Render //

static void renderer_set_viewport(renderer_t *renderer, f32 viewport_width, f32 viewport_height)
{
    renderer->viewport_width = viewport_width;
    renderer->viewport_height = viewport_height;
}

static void renderer_set_clear_color(renderer_t *renderer, f32 r, f32 g, f32 b, f32 a)
{
    renderer->clear.actions->color[0] = r;
    renderer->clear.actions->color[1] = g; 
    renderer->clear.actions->color[2] = b; 
    renderer->clear.actions->color[3] = a; 
}

static void renderer_draw_texture(render_queue_t *rq, gs_vec2 pos, gs_vec2 size,
                                  gs_handle(gs_graphics_texture_t) texture)
{
    int data_count = 16;
    f32 vertices[16] = {0}; // NOTE: size same as the data_count

    render_queue_item_t rqi = (render_queue_item_t) {
        .data_count = data_count,
        .offset = rq->buffer_used,
        .texture = texture
    };

    // TODO: Check if out of bounds

    gs_vec2 tl = pos;
    gs_vec2 br = gs_v2(pos.x + size.x, pos.y + size.y);

    // TODO: possibly color
    // Top Right
    vertices[0] = br.x;
    vertices[1] = tl.y;
    vertices[2] = 1.f;
    vertices[3] = 1.f;

    // Bottom Right
    vertices[4] = br.x;
    vertices[5] = br.y;
    vertices[6] = 1.f;
    vertices[7] = 0.f;

    // Bottom Left
    vertices[8] = tl.x;
    vertices[9] = br.y;
    vertices[10] = 0.f;
    vertices[11] = 0.f;

    // Top Left
    vertices[12] = tl.x;
    vertices[13] = tl.y;
    vertices[14] = 0.f;
    vertices[15] = 1.f;

    append_data_to_buffer(rq, &vertices, sizeof(vertices));
    rq->items[rq->count++] = rqi;
}

// Core //

static void init_renderer(renderer_t *renderer)
{
    renderer->command_buffer = gs_command_buffer_new(); 

    // TODO: Maybe arena instead of malloc
    int vertices_count = 16;
    int buffer_size = MAX_RENDER_OBJECTS * vertices_count * sizeof(f32);
    renderer->render_queue = (render_queue_t) {0};
    renderer->render_queue.buffer_size = buffer_size;
    renderer->render_queue.buffer = malloc(buffer_size);

    renderer->clear = (gs_graphics_clear_desc_t) {
        .actions = &(gs_graphics_clear_action_t) {
            .color = 0.f, 0.f, 0.f, 1.f
        }
    };

    renderer->u_projection = gs_graphics_uniform_create (
        &(gs_graphics_uniform_desc_t) {
            .stage = GS_GRAPHICS_SHADER_STAGE_VERTEX,
            .name = "u_projection",
            .layout = &(gs_graphics_uniform_layout_desc_t) {
                .type = GS_GRAPHICS_UNIFORM_MAT4
            }
        }
    );

    renderer->u_tex = gs_graphics_uniform_create (
        &(gs_graphics_uniform_desc_t) {
            .stage = GS_GRAPHICS_SHADER_STAGE_FRAGMENT,
            .name = "u_tex",
            .layout = &(gs_graphics_uniform_layout_desc_t) {
                .type = GS_GRAPHICS_UNIFORM_SAMPLER2D
            }
        }
    );

    float v_data[] = {
        // Positions   UVs
        0.5f,  0.5f,   1.f, 1.f, // top right
        0.5f, -0.5f,   1.f, 0.f, // bottom right
       -0.5f, -0.5f,   0.f, 0.f, // bottom left
       -0.5f,  0.5f,   0.f, 1.f  // top left 
    };

    renderer->texture_vbo = gs_graphics_vertex_buffer_create(
        &(gs_graphics_vertex_buffer_desc_t) {
            .data = v_data,
            .size = sizeof(v_data)
        }
    );

    u32 i_data[] = {
        0, 1, 3,
        1, 2, 3
    };

    renderer->texture_ibo = gs_graphics_index_buffer_create(
        &(gs_graphics_index_buffer_desc_t) {
            .data = i_data,
            .size = sizeof(i_data)
        }
    );

    // TODO: Move vertex and fragment shaders to seperate files
    renderer->texture_shader = gs_graphics_shader_create (
        &(gs_graphics_shader_desc_t) {
            .sources = (gs_graphics_shader_source_desc_t[]){
                {.type = GS_GRAPHICS_SHADER_STAGE_VERTEX, .source = v_src},
                {.type = GS_GRAPHICS_SHADER_STAGE_FRAGMENT, .source = f_src}
            }, 
            .size = 2 * sizeof(gs_graphics_shader_source_desc_t),
            .name = "quad"
        }
    );

    renderer->texture_pipeline = gs_graphics_pipeline_create (
        &(gs_graphics_pipeline_desc_t) {
            .raster = {
                .shader = renderer->texture_shader,
                .index_buffer_element_size = sizeof(uint32_t)
            },
            .blend = {
				.func = GS_GRAPHICS_BLEND_EQUATION_ADD,
				.src = GS_GRAPHICS_BLEND_MODE_SRC_ALPHA,
				.dst = GS_GRAPHICS_BLEND_MODE_ONE_MINUS_SRC_ALPHA
			},
			.depth = {
				.func = GS_GRAPHICS_DEPTH_FUNC_LESS
			},
            .layout = {
                .attrs = (gs_graphics_vertex_attribute_desc_t[]) {
                    {.format = GS_GRAPHICS_VERTEX_ATTRIBUTE_FLOAT2, .name = "a_pos"},
                    {.format = GS_GRAPHICS_VERTEX_ATTRIBUTE_FLOAT2, .name = "a_uv"}
                },
                .size = 2 * sizeof(gs_graphics_vertex_attribute_desc_t)
            }
        }
    );

    renderer->set_viewport = renderer_set_viewport;
    renderer->set_clear_color = renderer_set_clear_color;
    renderer->draw_texture = renderer_draw_texture;
    renderer->test_f = test_func;
}

static void begin_render_pass(renderer_t *renderer)
{
    const gs_vec2 fbs = gs_platform_framebuffer_sizev(gs_platform_main_window());
    renderer->projection = gs_mat4_ortho(0.f, fbs.x, fbs.y, 0.f, 0.f, 100.f);
    renderer->viewport_width = fbs.x;
    renderer->viewport_height = fbs.y;

    // Reset render_queue
    renderer->render_queue.count = 0;
    renderer->render_queue.buffer_used = 0;
}

// TODO: End Renderer
static void end_render_pass(renderer_t *renderer)
{
    gs_command_buffer_t *cb = &renderer->command_buffer;
    render_queue_t *rq = &renderer->render_queue;

    gs_graphics_renderpass_begin(cb, GS_GRAPHICS_RENDER_PASS_DEFAULT);
    gs_graphics_set_viewport(cb, 0, 0, (int32_t)renderer->viewport_width, (int32_t)renderer->viewport_height);
    gs_graphics_clear(cb, &renderer->clear);
    gs_graphics_pipeline_bind(cb, renderer->texture_pipeline);

    for (u32 i = 0; i < rq->count; ++i)
    {
        render_queue_item_t ri = rq->items[i];

        gs_graphics_vertex_buffer_desc_t updated_buffer = (gs_graphics_vertex_buffer_desc_t) {
            .data = (f32 *)((u8 *)rq->buffer + ri.offset),
            .size = sizeof(f32) * ri.data_count,
            .update = {
                .offset = 0,
                .type = GS_GRAPHICS_BUFFER_UPDATE_SUBDATA
            },
            .usage = GS_GRAPHICS_BUFFER_USAGE_DYNAMIC
        };

        gs_graphics_bind_uniform_desc_t uniforms[] = {
            (gs_graphics_bind_uniform_desc_t) {
                .uniform = renderer->u_projection,
                .data = &renderer->projection
            },
            (gs_graphics_bind_uniform_desc_t) {
                .uniform = renderer->u_tex,
                .data = &ri.texture
            }
        };

        gs_graphics_vertex_buffer_request_update(cb, renderer->texture_vbo, &updated_buffer);

        gs_graphics_bind_desc_t binds = (gs_graphics_bind_desc_t) {
            .vertex_buffers = {
                .desc = &(gs_graphics_bind_vertex_buffer_desc_t) {.buffer = renderer->texture_vbo}
            },
            .index_buffers = {
                .desc = &(gs_graphics_bind_index_buffer_desc_t) {.buffer = renderer->texture_ibo}
            },
            .uniforms = {
                .desc = uniforms,
                .size = sizeof(uniforms)
            }
        };

        gs_graphics_apply_bindings(cb, &binds);
        gs_graphics_draw(cb, &(gs_graphics_draw_desc_t){.start = 0, .count = 6});
    }

    gs_graphics_renderpass_end(cb);
    gs_graphics_command_buffer_submit(cb);
}

