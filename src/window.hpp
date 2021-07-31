#pragma once

#include <Windows.h>

namespace Window
{
        void init();
        void loop();
        void destroy();
        
        HWND handle();
}