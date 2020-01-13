/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/assets/json_asset.hpp>
#include <enduro2d/high/assets/text_asset.hpp>

namespace
{
    using namespace e2d;

    class json_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "json asset loading exception";
        }
    };
}

namespace e2d
{
    json_asset::load_async_result json_asset::load_async(
        const library& library, str_view address)
    {
        return library.load_asset_async<text_asset>(address)
        .then([
            address = str(address)
        ](const text_asset::load_result& json_data){
            return the<deferrer>().do_in_worker_thread([
                json_data,
                address = std::move(address)
            ](){
                E2D_PROFILER_SCOPE_EX("json_asset.parsing", {
                    {"address", address}
                });
                auto json = std::make_shared<rapidjson::Document>();
                if ( json->Parse(json_data->content().c_str()).HasParseError() ) {
                    throw json_asset_loading_exception();
                }
                return json_asset::create(std::move(json));
            });
        });
    }
}
