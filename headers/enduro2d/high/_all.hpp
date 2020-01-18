/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

#include "assets/atlas_asset.hpp"
#include "assets/binary_asset.hpp"
#include "assets/flipbook_asset.hpp"
#include "assets/font_asset.hpp"
#include "assets/image_asset.hpp"
#include "assets/json_asset.hpp"
#include "assets/material_asset.hpp"
#include "assets/mesh_asset.hpp"
#include "assets/model_asset.hpp"
#include "assets/prefab_asset.hpp"
#include "assets/script_asset.hpp"
#include "assets/shader_asset.hpp"
#include "assets/shape_asset.hpp"
#include "assets/sound_asset.hpp"
#include "assets/spine_asset.hpp"
#include "assets/sprite_asset.hpp"
#include "assets/text_asset.hpp"
#include "assets/texture_asset.hpp"
#include "assets/xml_asset.hpp"

#include "components/actor.hpp"
#include "components/behaviour.hpp"
#include "components/camera.hpp"
#include "components/colliders.hpp"
#include "components/commands.hpp"
#include "components/disabled.hpp"
#include "components/events.hpp"
#include "components/flipbook_player.hpp"
#include "components/label.hpp"
#include "components/model_renderer.hpp"
#include "components/named.hpp"
#include "components/renderer.hpp"
#include "components/rigid_body.hpp"
#include "components/scene.hpp"
#include "components/spine_player.hpp"
#include "components/sprite_renderer.hpp"
#include "components/touchable.hpp"

#include "systems/editor_system.hpp"
#include "systems/flipbook_system.hpp"
#include "systems/input_system.hpp"
#include "systems/label_system.hpp"
#include "systems/physics_system.hpp"
#include "systems/render_system.hpp"
#include "systems/script_system.hpp"
#include "systems/spine_system.hpp"

#include "address.hpp"
#include "asset.hpp"
#include "asset.inl"
#include "atlas.hpp"
#include "editor.hpp"
#include "factory.hpp"
#include "factory.inl"
#include "flipbook.hpp"
#include "gobject.hpp"
#include "inspector.hpp"
#include "inspector.inl"
#include "library.hpp"
#include "library.inl"
#include "luasol.hpp"
#include "model.hpp"
#include "node.hpp"
#include "node.inl"
#include "physics.hpp"
#include "prefab.hpp"
#include "script.hpp"
#include "spine.hpp"
#include "sprite.hpp"
#include "starter.hpp"
#include "world.hpp"
