/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "render.hpp"
#include "render_opengl_base.hpp"

#if defined(E2D_RENDER_MODE)
#if E2D_RENDER_MODE == E2D_RENDER_MODE_OPENGL || \
    E2D_RENDER_MODE == E2D_RENDER_MODE_OPENGLES || \
    E2D_RENDER_MODE == E2D_RENDER_MODE_OPENGLES3

namespace e2d
{
    //
    // shader::internal_state
    //

    class shader::internal_state final : private e2d::noncopyable {
    public:
        internal_state(
            debug& debug,
            opengl::gl_program_id id);
        ~internal_state() noexcept = default;
    public:
        debug& dbg() const noexcept;
        const opengl::gl_program_id& id() const noexcept;
    public:
        template < typename F >
        void with_uniform_location(str_hash name, F&& f) const;
        template < typename F >
        void with_attribute_location(str_hash name, F&& f) const;
    private:
        debug& debug_;
        opengl::gl_program_id id_;
        hash_map<str_hash, opengl::uniform_info> uniforms_;
        hash_map<str_hash, opengl::attribute_info> attributes_;
    };

    template < typename F >
    void shader::internal_state::with_uniform_location(str_hash name, F&& f) const {
        const auto iter = uniforms_.find(name);
        if ( iter != uniforms_.end() ) {
            stdex::invoke(std::forward<F>(f), iter->second);
        }
    }

    template < typename F >
    void shader::internal_state::with_attribute_location(str_hash name, F&& f) const {
        const auto iter = attributes_.find(name);
        if ( iter != attributes_.end() ) {
            stdex::invoke(std::forward<F>(f), iter->second);
        }
    }

    //
    // texture::internal_state
    //

    class texture::internal_state final : private e2d::noncopyable {
    public:
        internal_state(
            debug& debug,
            opengl::gl_texture_id id,
            const v2u& size,
            const pixel_declaration& decl);
        ~internal_state() noexcept = default;
    public:
        debug& dbg() const noexcept;
        const opengl::gl_texture_id& id() const noexcept;
        const v2u& size() const noexcept;
        const pixel_declaration& decl() const noexcept;
        void on_content_update(u32 frame_id) const noexcept;
    private:
        debug& debug_;
        opengl::gl_texture_id id_;
        v2u size_;
        pixel_declaration decl_;
        mutable u32 last_update_frame_id_ = 0;
    };

    //
    // index_buffer::internal_state
    //

    class index_buffer::internal_state final : private e2d::noncopyable {
    public:
        internal_state(
            debug& debug,
            opengl::gl_buffer_id id,
            std::size_t size,
            const index_declaration& decl);
        ~internal_state() noexcept = default;
    public:
        debug& dbg() const noexcept;
        const opengl::gl_buffer_id& id() const noexcept;
        std::size_t size() const noexcept;
        const index_declaration& decl() const noexcept;
        void on_content_update(u32 frame_id) const noexcept;
    private:
        debug& debug_;
        opengl::gl_buffer_id id_;
        std::size_t size_ = 0;
        index_declaration decl_;
        mutable u32 last_update_frame_id_ = 0;
    };

    //
    // vertex_buffer::internal_state
    //

    class vertex_buffer::internal_state final : private e2d::noncopyable {
    public:
        internal_state(
            debug& debug,
            opengl::gl_buffer_id id,
            std::size_t size);
        ~internal_state() noexcept = default;
    public:
        debug& dbg() const noexcept;
        const opengl::gl_buffer_id& id() const noexcept;
        std::size_t size() const noexcept;
        void on_content_update(u32 frame_id) const noexcept;
    private:
        debug& debug_;
        opengl::gl_buffer_id id_;
        std::size_t size_ = 0;
        mutable u32 last_update_frame_id_ = 0;
    };

    //
    // vertex_attribs::internal_state
    //

    class vertex_attribs::internal_state final : private e2d::noncopyable {
    public:
        internal_state(
            debug& debug,
            const vertex_declaration& decl);
        ~internal_state() noexcept = default;
    public:
        debug& dbg() const noexcept;
        const vertex_declaration& decl() const noexcept;
        bool operator==(const internal_state& r) const noexcept;
    private:
        debug& debug_;
        std::size_t hash_;
        vertex_declaration decl_;
    };
    
    //
    // const_buffer::internal_state
    //

    class const_buffer::internal_state final : private e2d::noncopyable {
    public:
        internal_state(
            debug& debug,
            opengl::gl_buffer_id id,
            std::size_t size);
        ~internal_state() noexcept = default;
    public:
        debug& dbg() const noexcept;
        const opengl::gl_buffer_id& id() const noexcept;
        std::size_t size() const noexcept;
        void on_content_update(u32 frame_id) const noexcept;
    private:
        debug& debug_;
        opengl::gl_buffer_id id_;
        std::size_t size_ = 0;
        mutable u32 last_update_frame_id_ = 0;
    };

    //
    // render_target::internal_state
    //

    class render_target::internal_state final : private e2d::noncopyable {
    public:
        internal_state(
            debug& debug,
            opengl::gl_framebuffer_id id,
            const v2u& size,
            texture_ptr color,
            texture_ptr depth,
            opengl::gl_renderbuffer_id color_rb,
            opengl::gl_renderbuffer_id depth_rb);
        ~internal_state() noexcept = default;
    public:
        debug& dbg() const noexcept;
        const opengl::gl_framebuffer_id& id() const noexcept;
        const v2u& size() const noexcept;
        const texture_ptr& color() const noexcept;
        const texture_ptr& depth() const noexcept;
        const opengl::gl_renderbuffer_id& color_rb() const noexcept;
        const opengl::gl_renderbuffer_id& depth_rb() const noexcept;
    private:
        debug& debug_;
        opengl::gl_framebuffer_id id_;
        v2u size_;
        texture_ptr color_;
        texture_ptr depth_;
        opengl::gl_renderbuffer_id color_rb_;
        opengl::gl_renderbuffer_id depth_rb_;
    };

    //
    // render::internal_state
    //

    class render::internal_state final : private e2d::noncopyable {
    public:
        internal_state(
            debug& debug,
            window& window);
        ~internal_state() noexcept = default;
    public:
        debug& dbg() const noexcept;
        window& wnd() const noexcept;
        const device_caps& device_capabilities() const noexcept;
        const opengl::gl_device_caps& device_capabilities_ext() const noexcept;
        const render_target_ptr& render_target() const noexcept;
        statistics& stats() noexcept;
        const statistics& last_stats() const noexcept;
        u32 frame_id() const noexcept;
        bool inside_render_pass() const noexcept;

        void on_present() noexcept;
    public:
        internal_state& reset_states() noexcept;
        internal_state& set_states(const state_block& sb) noexcept;
        
        internal_state& reset_shader_program() noexcept;
        internal_state& set_shader_program(const shader_ptr& sp) noexcept;
        
        void begin_render_pass(const renderpass_desc& rp) noexcept;
        void end_render_pass() noexcept;
        
        void bind_index_buffer(
            const index_buffer_ptr& ibuffer) noexcept;
        void bind_vertex_buffer(
            std::size_t index,
            const vertex_buffer_ptr& vbuffer,
            const vertex_attribs_ptr& attribs,
            std::size_t offset) noexcept;
        void bind_const_buffer(
            const_buffer::scope scope,
            const const_buffer_ptr& cbuffer) noexcept;
        void bind_textures(
            sampler_block::scope scope,
            const sampler_block& samplers) noexcept;

        void draw(topology topo, u32 first, u32 count) noexcept;
        void draw_indexed(topology topo, u32 first, u32 count) noexcept;
        
        vertex_attribs_ptr create_vertex_attribs(
            const vertex_declaration& decl);
    private:
        void set_depth_state_(const depth_state& ds) noexcept;
        void set_stencil_state_(const stencil_state& ss) noexcept;
        void set_rasterization_state(const rasterization_state& rs) noexcept;
        void set_blending_state_(const blending_state& bs) noexcept;
        void set_render_target_(const render_target_ptr& rt) noexcept;
        void reset_render_target_() noexcept;
        void create_debug_output_() noexcept;
        static void GLAPIENTRY debug_output_callback_(
            GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar* message,
            const void* userParam);
    private:
        enum class dirty_flag : u32 {
            vertex_buffer = 1 << 0,
            index_buffer = 1 << 1,
            const_buffer = 1 << 2,
            textures = 1 << 3,
            shader = 1 << 4,
        };
    private:
        debug& debug_;
        window& window_;

        device_caps device_caps_;
        opengl::gl_device_caps device_caps_ext_;
        opengl::gl_program_id default_sp_;
        opengl::gl_framebuffer_id default_fb_;

        // graphics pipeline states
        state_block state_block_;
        shader_ptr shader_program_;
        
        // render pass states
        render_target_ptr render_target_;
        b2u render_area_;
        attachment_store_op color_store_op_ = attachment_store_op::store;
        attachment_store_op depth_store_op_ = attachment_store_op::discard;
        attachment_store_op stencil_store_op_ = attachment_store_op::discard;
        bool inside_render_pass_ = false;

        // graphics resource bindings
        struct vb_binding {
            vertex_buffer_ptr buffer;
            vertex_attribs_ptr attribs;
            std::size_t offset;
            bool dirty = true;
        };
        std::array<vb_binding, 8> vertex_buffers_;
        std::array<const_buffer_ptr, u32(const_buffer::scope::last_)> cbuffers_;
        dirty_flag dirty_flags_;

        // statistic
        statistics current_stat_;
        statistics last_frame_stat_;
        u32 frame_id_ = 1;

        flat_set<vertex_attribs_ptr> vertex_attrib_cache_;
    };
}

#endif
#endif
