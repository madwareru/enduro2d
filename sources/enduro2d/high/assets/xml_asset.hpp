/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include <enduro2d/high/assets/assets.hpp>

#include <3rdparty/pugixml/pugixml.hpp>

namespace e2d
{
    class xml_asset final : public content_asset<xml_asset, pugi::xml_document> {
    public:
        using content_asset<xml_asset, pugi::xml_document>::content_asset;
        static load_async_result load_async(library& library, str_view address);
    };
}
