/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../common.hpp"
using namespace e2d;

namespace
{
    class game_system final : public ecs::system {
    public:
        void process(ecs::registry& owner) override {
            E2D_UNUSED(owner);
            const keyboard& k = the<input>().keyboard();

            if ( k.is_key_just_released(keyboard_key::f12) ) {
                the<dbgui>().toggle_visible(!the<dbgui>().visible());
            }

            if ( k.is_key_just_released(keyboard_key::escape) ) {
                the<window>().set_should_close(true);
            }

            if ( k.is_key_pressed(keyboard_key::lsuper) && k.is_key_just_released(keyboard_key::enter) ) {
                the<window>().toggle_fullscreen(!the<window>().fullscreen());
            }

            owner.for_joined_components<ui_controller_events, ui_scrollable, name_comp>(
            [this](const ecs::entity& e, const ui_controller_events& events, const ui_scrollable&, const name_comp& name) {
                if ( name.name() != str_hash("scrollable") ) {
                    return;
                }
                for ( auto& ev : events.events() ) {
                    if ( auto* upd = std::any_cast<ui_scrollable::scroll_update_evt>(&ev) ) {
                        if ( math::abs(upd->overscroll.y) > 0.1f ) {
                            overscroll_animation_(e, upd->overscroll.y);
                        }
                    }
                }
            });
        }
    private:
        void overscroll_animation_(const ecs::entity&, f32 ov) {
            auto objs = the<world>().find_gobject(ov > 0.0f ? "viewport.top" : "viewport.bottom");
            if ( objs.empty() ) {
                return;
            }
            if ( objs.front()->entity().find_component<ui_animation>() ) {
                return;
            }
            auto& anim = objs.front()->entity().assign_component<ui_animation>();
            anim.add(ui_animation::custom([](f32 f, ecs::entity& e) {
                    auto& spr = e.get_component<sprite_renderer>();
                    color32 c = spr.tint();
                    c.a = u8(255.0f * math::sin(f * math::pi<f32>()));
                    spr.tint(c);
                })
                .duration(secf(2.0f)));
        }
    };

    class camera_system final : public ecs::system {
    public:
        void process(ecs::registry& owner) override {
            owner.for_each_component<camera>(
            [](const ecs::const_entity&, camera& cam){
                if ( !cam.target() ) {
                    cam.viewport(
                        the<window>().real_size());
                    cam.projection(math::make_orthogonal_lh_matrix4(
                        the<window>().real_size().cast_to<f32>(), 0.f, 1000.f));
                }
            });
        }
    };

    class game final : public starter::application {
    public:
        bool initialize() final {
            return create_scene()
                && create_camera()
                && create_systems();
        }
    private:
        bool create_scene() {
            auto scene_prefab_res = the<library>().load_asset<prefab_asset>("scenes/ui_prototype.json");
            auto scene_go = scene_prefab_res
                ? the<world>().instantiate(scene_prefab_res->content())
                : nullptr;
            return !!scene_go;
        }

        bool create_camera() {
            auto camera_i = the<world>().instantiate();
            camera_i->entity_filler()
                .component<camera>(camera()
                    .background({0.f, 0.f, 0.f, 1.f}))
                .component<actor>(node::create(camera_i))
                .component<camera::input_handler_tag>();
            return true;
        }

        bool create_systems() {
            ecs::registry_filler(the<world>().registry())
                .system<game_system, world_ev::update>()
                .system<camera_system, world_ev::pre_update>();
            return true;
        }
    };
}

int e2d_main(int argc, char *argv[]) {
    const auto starter_params = starter::parameters(
        engine::parameters("sample_10", "enduro2d")
            .timer_params(engine::timer_parameters()
                .maximal_framerate(100)));
    modules::initialize<starter>(argc, argv, starter_params).start<game>();
    modules::shutdown<starter>();
    return 0;
}
