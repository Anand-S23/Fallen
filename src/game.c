#include "game.h"

static void render_game(gs_command_buffer_t *cb, game_data_t *data)
{
}

// Core //

static void init_game(app_t *app)
{
    gs_command_buffer_t *cb = &app->cb;
    game_data_t *data = &app->game_data;

    int32_t width, height, num_comps;
    void* texture_data = NULL;
    gs_util_load_texture_data_from_file("./assets/characters.png",
                                        &width, &height, &num_comps,
                                        &texture_data, 1);

    gs_vec2 position = gs_v2(10, 10);
    gs_vec2 tl = position;
    gs_vec2 br = gs_v2(position.x + width, position.y + height);

    // Top Right
    data->vertices[0] = br.x;
    data->vertices[1] = tl.y;

    data->vertices[2] = 1.f;
    data->vertices[3] = 1.f;

    // Bottom Right
    data->vertices[4] = br.x;
    data->vertices[5] = br.y;

    data->vertices[6] = 1.f;
    data->vertices[7] = 0.f;

    // Bottom Left
    data->vertices[8] = tl.x;
    data->vertices[9] = br.y;

    data->vertices[10] = 0.f;
    data->vertices[11] = 0.f;

    // Top Left
    data->vertices[12] = tl.x;
    data->vertices[13] = tl.y;

    data->vertices[14] = 0.f;
    data->vertices[15] = 1.f;


    // Create dynamic texture
    data->tex = gs_graphics_texture_create (
        &(gs_graphics_texture_desc_t) {
            .type = GS_GRAPHICS_TEXTURE_2D, 
            .width = width,
            .height = height, 
            .data = texture_data,
            .format = GS_GRAPHICS_TEXTURE_FORMAT_RGBA8,
            .min_filter = GS_GRAPHICS_TEXTURE_FILTER_NEAREST, 
            .mag_filter = GS_GRAPHICS_TEXTURE_FILTER_NEAREST
        }
    );

    data->u_projection = gs_graphics_uniform_create (
        &(gs_graphics_uniform_desc_t) {
            .stage = GS_GRAPHICS_SHADER_STAGE_VERTEX,
            .name = "u_projection",
            .layout = &(gs_graphics_uniform_layout_desc_t) {
                .type = GS_GRAPHICS_UNIFORM_MAT4
            }
        }
    );

    // Construct sampler buffer
    data->u_tex = gs_graphics_uniform_create (
        &(gs_graphics_uniform_desc_t) {
            .stage = GS_GRAPHICS_SHADER_STAGE_FRAGMENT,
            .name = "u_tex",
            .layout = &(gs_graphics_uniform_layout_desc_t) {
                .type = GS_GRAPHICS_UNIFORM_SAMPLER2D
            }
        }
    );

    // Construct vertex buffer
    data->vbo = gs_graphics_vertex_buffer_create(
        &(gs_graphics_vertex_buffer_desc_t) {
            .data = v_data,
            .size = sizeof(v_data)
        }
    );

    gs_graphics_vertex_buffer_desc_t updated_buffer_desc = (gs_graphics_vertex_buffer_desc_t) {
		.data = data->vertices,
		.size = sizeof(data->vertices),
		.update = {
			.offset = 0,
			.type = GS_GRAPHICS_BUFFER_UPDATE_SUBDATA
		},
		.usage = GS_GRAPHICS_BUFFER_USAGE_DYNAMIC
	};

	gs_graphics_vertex_buffer_request_update(cb, data->vbo, &updated_buffer_desc);


    // Construct index buffer
    data->ibo = gs_graphics_index_buffer_create(
        &(gs_graphics_index_buffer_desc_t) {
            .data = i_data,
            .size = sizeof(i_data)
        }
    );

    // Create shader
    data->shader = gs_graphics_shader_create (
        &(gs_graphics_shader_desc_t) {
            .sources = (gs_graphics_shader_source_desc_t[]){
                {.type = GS_GRAPHICS_SHADER_STAGE_VERTEX, .source = v_src},
                {.type = GS_GRAPHICS_SHADER_STAGE_FRAGMENT, .source = f_src}
            }, 
            .size = 2 * sizeof(gs_graphics_shader_source_desc_t),
            .name = "quad"
        }
    );

    data->pip = gs_graphics_pipeline_create (
        &(gs_graphics_pipeline_desc_t) {
            .raster = {
                .shader = data->shader,
                .index_buffer_element_size = sizeof(uint32_t)
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
}

static void update_game(app_t *app)
{
    gs_command_buffer_t *cb = &app->cb;
    game_data_t *data = &app->game_data;

    const gs_vec2 fbs = gs_platform_framebuffer_sizev(gs_platform_main_window());
    gs_mat4 projection = gs_mat4_ortho(0.f, fbs.x, fbs.y, 0.f, 0.f, 100.f);

    gs_graphics_clear_desc_t clear = (gs_graphics_clear_desc_t) {
        .actions = &(gs_graphics_clear_action_t) {
            .color = 0.1f, 0.1f, 0.1f, 1.f
        }
    };

    gs_graphics_bind_uniform_desc_t uniforms[] = {
        (gs_graphics_bind_uniform_desc_t) {
            .uniform = data->u_projection,
            .data = &projection
        },
        (gs_graphics_bind_uniform_desc_t) {
            .uniform = data->u_tex,
            .data = &data->tex
        }
    };

    gs_graphics_bind_desc_t binds = {
        .vertex_buffers = {
            .desc = &(gs_graphics_bind_vertex_buffer_desc_t) {.buffer = data->vbo}
        },
        .index_buffers = {
            .desc = &(gs_graphics_bind_index_buffer_desc_t) {.buffer = data->ibo}
        },
        .uniforms = {
            .desc = uniforms,
            .size = sizeof(uniforms)
        }
    };

    gs_graphics_renderpass_begin(cb, GS_GRAPHICS_RENDER_PASS_DEFAULT);
        gs_graphics_set_viewport(cb, 0, 0, (int32_t)fbs.x, (int32_t)fbs.y);
        gs_graphics_clear(cb, &clear);
        gs_graphics_pipeline_bind(cb, data->pip);
        gs_graphics_apply_bindings(cb, &binds);
        gs_graphics_draw(cb, &(gs_graphics_draw_desc_t){.start = 0, .count = 6});
    gs_graphics_renderpass_end(cb);

    gs_graphics_command_buffer_submit(cb);
}

