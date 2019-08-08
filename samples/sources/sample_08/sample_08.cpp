/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../common.hpp"
using namespace e2d;

namespace
{
    const char* vs1_source_cstr = R"glsl(
        attribute vec2 a_position;
        attribute vec2 a_uv;
        attribute vec4 a_color;

        uniform mat4 u_MVP;

        varying vec4 v_color;
        varying vec2 v_uv;

        void main(){
          v_color = a_color;
          v_uv = a_uv;
          gl_Position = vec4(a_position, 0.0, 1.0) * u_MVP;
        }
    )glsl";

    const char* fs1_source_cstr = R"glsl(
        uniform sampler2D u_texture;
        varying vec4 v_color;
        varying vec2 v_uv;

        void main(){
            gl_FragColor = v_color * texture2D(u_texture, v_uv);
        }
    )glsl";
    
    const char* vs2_source_cstr = R"glsl(
        attribute vec3 a_position;
        attribute vec4 a_color;

        uniform mat4 u_MVP;

        varying vec4 v_color;

        void main(){
          v_color = a_color;
          gl_Position = vec4(a_position, 1.0) * u_MVP;
        }
    )glsl";

    const char* fs2_source_cstr = R"glsl(
        varying vec4 v_color;

        void main(){
            gl_FragColor = v_color;
        }
    )glsl";

    struct vertex {
        v2f position;
        v2f uv;
        color32 color;

        vertex(const v2f& pos, const v2f& uv, color32 col)
        : position(pos)
        , uv(uv)
        , color(col) {}

        static vertex_declaration decl() noexcept {
            return vertex_declaration()
                .add_attribute<v2f>("a_position")
                .add_attribute<v2f>("a_uv")
                .add_attribute<color32>("a_color").normalized();
        }
    };
    
    struct vertex2 {
        v3f position;
        color32 color;

        vertex2(const v3f& pos, color32 col)
        : position(pos)
        , color(col) {}

        static vertex_declaration decl() noexcept {
            return vertex_declaration()
                .add_attribute<v3f>("a_position")
                .add_attribute<color32>("a_color").normalized();
        }
    };
    
    template < typename VertexType >
    class rectangle_batch_strip {
    public:
        using vertex_type = VertexType;
    public:
        rectangle_batch_strip() = default;
        rectangle_batch_strip(const b2f& pos, const b2f& uv, color32 col)
        : pos(pos), uv(uv), col(col) {}

        void get_indices(batcher::index_iterator iter) const noexcept {
            iter++ = 0;  iter++ = 1; iter++ = 2;  iter++ = 3;
        }

        void get_vertices(batcher::vertex_iterator<VertexType> iter) const noexcept {
            iter[0] = VertexType(pos.position,                         uv.position,                        col);
            iter[1] = VertexType(pos.position + v2f(0.0f, pos.size.y), uv.position + v2f(0.0f, uv.size.y), col);
            iter[2] = VertexType(pos.position + v2f(pos.size.x, 0.0f), uv.position + v2f(uv.size.x, 0.0f), col);
            iter[3] = VertexType(pos.position + pos.size,              uv.position + uv.size,              col);
        }

        static batcher::topology topology() noexcept { return batcher::topology::triangles_strip; }
        static u32 index_count() noexcept { return 4; }
        static u32 vertex_count() noexcept { return 4; }
    public:
        b2f pos;
        b2f uv;
        color32 col;
    };

    using rect_batch = batcher::rectangle_batch<vertex>;
    using rect_batch_strip = rectangle_batch_strip<vertex>;


    class game final : public engine::application {
    public:
        bool initialize() final {
            shader1_ = the<render>().create_shader(
                vs1_source_cstr, fs1_source_cstr);
            shader2_ = the<render>().create_shader(
                vs2_source_cstr, fs2_source_cstr);
            texture1_ = the<render>().create_texture(
                the<vfs>().read(url("resources://bin/library/cube_0.png")));
            texture2_ = the<render>().create_texture(
                the<vfs>().read(url("resources://bin/library/cube_1.png")));
            texture3_ = the<render>().create_texture(
                the<vfs>().read(url("resources://bin/library/ship.png")));

            if ( !shader1_ || !shader1_ || !texture1_ || !texture2_ || !texture3_ ) {
                return false;
            }

            return true;
        }

        bool frame_tick() final {
            const keyboard& k = the<input>().keyboard();

            if ( the<window>().should_close() || k.is_key_just_released(keyboard_key::escape) ) {
                return false;
            }

            /*if ( k.is_key_just_pressed(keyboard_key::f12) ) {
                the<dbgui>().toggle_visible(!the<dbgui>().visible());
            }*/

            if ( k.is_key_pressed(keyboard_key::lsuper) && k.is_key_just_released(keyboard_key::enter) ) {
                the<window>().toggle_fullscreen(!the<window>().fullscreen());
            }

            return true;
        }

        void frame_render() final {
            const auto framebuffer_size = the<window>().real_size().cast_to<f32>();
            const auto projection = math::make_orthogonal_lh_matrix4(
                framebuffer_size, 0.f, 1.f);

            auto& the_batcher = the<batcher>();
            auto& the_render = the<render>();

            the_render.begin_pass(render::renderpass_desc()
                .color_clear({0.f, 0.0f, 0.f, 1.f})
                .color_store()
                .depth_clear(1.0f)
                .depth_discard()
                .viewport(the<window>().real_size()));

            batcher::material mtr1 = batcher::material()
                .shader(shader1_)
                .blend(batcher::blend_mode()
                    .src_factor(render::blending_factor::src_alpha)
                    .dst_factor(render::blending_factor::one_minus_src_alpha))
                .property("u_MVP", projection)
                .sampler("u_texture", render::sampler_state()
                    .texture(texture1_)
                    .min_filter(render::sampler_min_filter::linear)
                    .mag_filter(render::sampler_mag_filter::linear));

            batcher::material mtr2 = batcher::material()
                .shader(shader1_)
                .blend(batcher::blend_mode()
                    .src_factor(render::blending_factor::src_alpha)
                    .dst_factor(render::blending_factor::one_minus_src_alpha))
                .property("u_MVP", projection)
                .sampler("u_texture", render::sampler_state()
                    .texture(texture2_)
                    .min_filter(render::sampler_min_filter::linear)
                    .mag_filter(render::sampler_mag_filter::linear));

            auto batch = the_batcher.alloc_batch<vertex2>(4, 6,
                batcher::topology::triangles,
                batcher::material()
                    .shader(shader2_)
                    .blend(batcher::blend_mode()
                        .src_factor(render::blending_factor::src_alpha)
                        .dst_factor(render::blending_factor::one_minus_src_alpha))
                    .property("u_MVP", projection));
            batch.vertices[0] = vertex2(v3f(- 90.0f,  170.0f, 0.0f), color32::red());
            batch.vertices[1] = vertex2(v3f(-120.0f, -210.0f, 0.0f), color32::green());
            batch.vertices[2] = vertex2(v3f( 120.0f,  230.0f, 0.0f), color32::blue());
            batch.vertices[3] = vertex2(v3f(  80.0f, -130.0f, 0.0f), color32::yellow());
            batch.indices++ = 0;  batch.indices++ = 1;  batch.indices++ = 2;
            batch.indices++ = 1;  batch.indices++ = 2;  batch.indices++ = 3;

            the_batcher.add_batch(
                mtr1,
                rect_batch(
                    b2f(100.0f, -50.0f, 100.0f, 100.0f),
                    b2f(0.0f, 0.0f, 1.0f, -1.0f),
                    color32::green()));
            
            the_batcher.add_batch(
                mtr1,
                rect_batch(
                    b2f(50.0f, 50.0f, 100.0f, 100.0f),
                    b2f(0.0f, 0.0f, 1.0f, -1.0f),
                    color32::green()));
            
            the_batcher.add_batch(
                mtr2,
                rect_batch_strip(
                    b2f(-200.0f, -50.0f, 100.0f, 100.0f),
                    b2f(0.0f, 0.0f, 1.0f, -1.0f),
                    color32::blue()));
            
            the_batcher.add_batch(
                mtr2,
                rect_batch_strip(
                    b2f(-250.0f, -180.0f, 100.0f, 100.0f),
                    b2f(0.0f, 0.0f, 1.0f, -1.0f),
                    color32::blue()));
            
            the_batcher.add_batch(
                mtr2,
                rect_batch_strip(
                    b2f(-170.0f, 130.0f, 100.0f, 100.0f),
                    b2f(0.0f, 0.0f, 1.0f, -1.0f),
                    color32::blue()));

            the_batcher.flush();
            the_render.end_pass();
        }
    private:
        shader_ptr shader1_;
        shader_ptr shader2_;
        texture_ptr texture1_;
        texture_ptr texture2_;
        texture_ptr texture3_;
    };
}

int e2d_main(int argc, char *argv[]) {
    auto params = engine::parameters("sample_00", "enduro2d")
        .timer_params(engine::timer_parameters()
            .maximal_framerate(100));
    modules::initialize<engine>(argc, argv, params).start<game>();
    modules::shutdown<engine>();
    return 0;
}
