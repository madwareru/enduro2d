/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "debug.hpp"

#if defined(E2D_DEBUG_NATIVE_LOG_MODE) && E2D_DEBUG_NATIVE_LOG_MODE == E2D_DEBUG_NATIVE_LOG_MODE_MSVC

#include <Windows.h>

namespace e2d
{
    bool debug_native_log_sink::on_message(debug::level lvl, str_view text) noexcept {
        // TODO: should be <file>(<line>): <message> <newline>
        OutputDebugStringA(text.data());
        OutputDebugStringA("\n");
        return true;
    }
}

#endif