/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../factory.hpp"

namespace e2d
{
    template < u32 N >
    class convex_hull_screenspace_collider_base {
    public:
        struct plane2d {
            v2f norm;
            f32 dist;
        };
    public:
        convex_hull_screenspace_collider_base() = default;
    public:
        std::array<plane2d, N> planes;
    };

    class convex_hull_screenspace_collider4 final :
        public convex_hull_screenspace_collider_base<4>
    {};
}
