/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../library.hpp"

namespace e2d
{
    using xml_uptr = std::unique_ptr<pugi::xml_document>;
    class xml_asset final : public content_asset<xml_asset, xml_uptr> {
    public:
        static load_async_result load_async(const library& library, str_view address);
    };
}
