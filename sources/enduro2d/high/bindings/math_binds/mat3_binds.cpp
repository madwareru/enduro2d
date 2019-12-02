/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_math_binds.hpp"

namespace
{
    using namespace e2d;

    template < typename T >
    void bind_mat3_t(const str& name, sol::state& l) {
        l.new_usertype<mat3<T>>(name,
            sol::constructors<
                mat3<T>(),
                mat3<T>(const vec3<T>&, const vec3<T>&, const vec3<T>&)
            >(),

            "zero", &mat3<T>::zero,
            "identity", &mat3<T>::identity,

            "rows", &mat3<T>::rows,

            sol::meta_function::to_string, [](const mat3<T>& v){
                return strings::rformat("%0", v);
            },

            sol::meta_function::equal_to, sol::resolve<bool(const mat3<T>&, const mat3<T>&)>(::operator==),
            sol::meta_function::unary_minus, sol::resolve<mat3<T>(const mat3<T>&)>(::operator-),

            sol::meta_function::addition, sol::overload(
                sol::resolve<mat3<T>(T, const mat3<T>&)>(::operator+),
                sol::resolve<mat3<T>(const mat3<T>&, T)>(::operator+),
                sol::resolve<mat3<T>(const mat3<T>&, const mat3<T>&)>(::operator+)),

            sol::meta_function::multiplication, sol::overload(
                sol::resolve<mat3<T>(T, const mat3<T>&)>(::operator*),
                sol::resolve<mat3<T>(const mat3<T>&, T)>(::operator*),
                sol::resolve<mat3<T>(const mat3<T>&, const mat3<T>&)>(::operator*)),

            "make_scale", sol::overload(
                sol::resolve<mat3<T>(T,T,T)>(&math::make_scale_matrix3),
                sol::resolve<mat3<T>(const vec4<T>&)>(&math::make_scale_matrix3),
                sol::resolve<mat3<T>(const vec3<T>&)>(&math::make_scale_matrix3),
                sol::resolve<mat3<T>(const vec2<T>&, T)>(&math::make_scale_matrix3)),

            "make_rotation", sol::overload(
                [](T angle, T x, T y, T z) -> mat3<T> { return math::make_rotation_matrix3(make_rad(angle), x, y, z); },
                [](T angle, const vec4<T>& xyz) -> mat3<T> { return math::make_rotation_matrix3(make_rad(angle), xyz); },
                [](T angle, const vec3<T>& xyz) -> mat3<T> { return math::make_rotation_matrix3(make_rad(angle), xyz); },
                [](T angle, const vec2<T>& xy, T z) -> mat3<T> { return math::make_rotation_matrix3(make_rad(angle), xy, z); },
                [](const quat<T>& q) -> mat3<T> { return math::make_rotation_matrix3(q); }),

            "approximately", [](const mat3<T>& l, const mat3<T>& r) -> bool {
                return math::approximately(l,r);
            },

            "inversed", [](const mat3<T>& m) -> std::pair<mat3<T>, bool> {
                return math::inversed(m);
            },

            "transposed", [](const mat3<T>& m) -> mat3<T> {
                return math::transposed(m);
            },

            "contains_nan", [](const mat3<T>& m) -> bool {
                return math::contains_nan(m);
            }
        );
    }
}

namespace e2d::bindings::math
{
    void bind_mat3(sol::state& l) {
        bind_mat3_t<f32>("m3f", l);
    }
}