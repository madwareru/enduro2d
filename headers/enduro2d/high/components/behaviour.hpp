/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../factory.hpp"
#include "../assets/script_asset.hpp"

namespace e2d
{
    class behaviour final {
    public:
        behaviour() = default;
        behaviour(const script_asset::ptr& script);

        behaviour& script(const script_asset::ptr& value) noexcept;
        [[nodiscard]] const script_asset::ptr& script() const noexcept;
    private:
        script_asset::ptr script_;
    };

    template <>
    class factory_loader<behaviour> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            behaviour& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    inline behaviour::behaviour(const script_asset::ptr& value)
    : script_(value) {}

    inline behaviour& behaviour::script(const script_asset::ptr& value) noexcept {
        script_ = value;
        return *this;
    }

    inline const script_asset::ptr& behaviour::script() const noexcept {
        return script_;
    }
}
