/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_systems.hpp"

namespace e2d
{
    class camera_system final
        : public ecs::system<ecs::after<systems::update_event>> {
    public:
        camera_system();
        ~camera_system() noexcept;

        void process(
            ecs::registry& owner,
            const ecs::after<systems::update_event>& trigger) override;
    private:
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };
}
