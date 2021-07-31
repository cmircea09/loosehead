#pragma once

#include "strict_win.hpp"

namespace Window
{
        void init();
        void loop();
        void destroy();
        
        HWND handle();
}