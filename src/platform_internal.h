//========================================================================
// GLFW 3.4 - www.glfw.org
//------------------------------------------------------------------------
// Copyright (c) 2019 
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================

#ifndef _GLFW_WIN32_PLATFORM_WINDOW_STATE
#define _GLFW_WIN32_PLATFORM_WINDOW_STATE
#endif
#ifndef _GLFW_WL_PLATFORM_WINDOW_STATE
#define _GLFW_WL_PLATFORM_WINDOW_STATE
#endif
#ifndef _GLFW_X11_PLATFORM_WINDOW_STATE
#define _GLFW_X11_PLATFORM_WINDOW_STATE
#endif
#ifndef _GLFW_COCOA_PLATFORM_WINDOW_STATE
#define _GLFW_COCOA_PLATFORM_WINDOW_STATE
#endif

#define _GLFW_PLATFORM_WINDOW_STATE         \
    _GLFW_WIN32_PLATFORM_WINDOW_STATE;      \
    _GLFW_X11_PLATFORM_WINDOW_STATE;        \
    _GLFW_WL_PLATFORM_WINDOW_STATE;         \
    _GLFW_COCOA_PLATFORM_WINDOW_STATE;


#ifndef _GLFW_WIN32_PLATFORM_LIBRARY_WINDOW_STATE
#define _GLFW_WIN32_PLATFORM_LIBRARY_WINDOW_STATE
#endif
#ifndef _GLFW_WL_PLATFORM_LIBRARY_WINDOW_STATE
#define _GLFW_WL_PLATFORM_LIBRARY_WINDOW_STATE
#endif
#ifndef _GLFW_X11_PLATFORM_LIBRARY_WINDOW_STATE
#define _GLFW_X11_PLATFORM_LIBRARY_WINDOW_STATE
#endif
#ifndef _GLFW_COCOA_PLATFORM_LIBRARY_WINDOW_STATE
#define _GLFW_COCOA_PLATFORM_LIBRARY_WINDOW_STATE
#endif

#define _GLFW_PLATFORM_LIBRARY_WINDOW_STATE         \
    _GLFW_WIN32_PLATFORM_LIBRARY_WINDOW_STATE;      \
    _GLFW_X11_PLATFORM_LIBRARY_WINDOW_STATE;        \
    _GLFW_WL_PLATFORM_LIBRARY_WINDOW_STATE;         \
    _GLFW_COCOA_PLATFORM_LIBRARY_WINDOW_STATE;


#ifndef _GLFW_WIN32_PLATFORM_MONITOR_STATE
#define _GLFW_WIN32_PLATFORM_MONITOR_STATE
#endif
#ifndef _GLFW_WL_PLATFORM_MONITOR_STATE
#define _GLFW_WL_PLATFORM_MONITOR_STATE
#endif
#ifndef _GLFW_X11_PLATFORM_MONITOR_STATE
#define _GLFW_X11_PLATFORM_MONITOR_STATE
#endif
#ifndef _GLFW_COCOA_PLATFORM_MONITOR_STATE
#define _GLFW_COCOA_PLATFORM_MONITOR_STATE
#endif

#define _GLFW_PLATFORM_MONITOR_STATE         \
    _GLFW_WIN32_PLATFORM_MONITOR_STATE;      \
    _GLFW_X11_PLATFORM_MONITOR_STATE;        \
    _GLFW_WL_PLATFORM_MONITOR_STATE;         \
    _GLFW_COCOA_PLATFORM_MONITOR_STATE;


#ifndef _GLFW_WIN32_PLATFORM_CURSOR_STATE
#define _GLFW_WIN32_PLATFORM_CURSOR_STATE
#endif
#ifndef _GLFW_WL_PLATFORM_CURSOR_STATE
#define _GLFW_WL_PLATFORM_CURSOR_STATE
#endif
#ifndef _GLFW_X11_PLATFORM_CURSOR_STATE
#define _GLFW_X11_PLATFORM_CURSOR_STATE
#endif
#ifndef _GLFW_COCOA_PLATFORM_CURSOR_STATE
#define _GLFW_COCOA_PLATFORM_CURSOR_STATE
#endif

#define _GLFW_PLATFORM_CURSOR_STATE         \
    _GLFW_WIN32_PLATFORM_CURSOR_STATE;      \
    _GLFW_X11_PLATFORM_CURSOR_STATE;        \
    _GLFW_WL_PLATFORM_CURSOR_STATE;         \
    _GLFW_COCOA_PLATFORM_CURSOR_STATE;

