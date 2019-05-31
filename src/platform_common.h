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

#pragma once

typedef struct _GLFWplatformfunctions
{
    int (*_glfwPlatformInitFunc)(void);
    void (*_glfwPlatformTerminateFunc)(void);
    const char* (*_glfwPlatformGetVersionStringFunc)(void);

    void (*_glfwPlatformGetCursorPosFunc)(_GLFWwindow* window, double* xpos, double* ypos);
    void (*_glfwPlatformSetCursorPosFunc)(_GLFWwindow* window, double xpos, double ypos);
    void (*_glfwPlatformSetCursorModeFunc)(_GLFWwindow* window, int mode);
    void (*_glfwPlatformSetRawMouseMotionFunc)(_GLFWwindow *window, GLFWbool enabled);
    GLFWbool (*_glfwPlatformRawMouseMotionSupportedFunc)(void);
    int (*_glfwPlatformCreateCursorFunc)(_GLFWcursor* cursor,
                                         const GLFWimage* image, int xhot, int yhot);
    int (*_glfwPlatformCreateStandardCursorFunc)(_GLFWcursor* cursor, int shape);
    void (*_glfwPlatformDestroyCursorFunc)(_GLFWcursor* cursor);
    void (*_glfwPlatformSetCursorFunc)(_GLFWwindow* window, _GLFWcursor* cursor);

    const char* (*_glfwPlatformGetScancodeNameFunc)(int scancode);
    int (*_glfwPlatformGetKeyScancodeFunc)(int key);

    void (*_glfwPlatformFreeMonitorFunc)(_GLFWmonitor* monitor);
    void (*_glfwPlatformGetMonitorPosFunc)(_GLFWmonitor* monitor, int* xpos, int* ypos);
    void (*_glfwPlatformGetMonitorContentScaleFunc)(_GLFWmonitor* monitor,
                                                    float* xscale, float* yscale);
    void (*_glfwPlatformGetMonitorWorkareaFunc)(_GLFWmonitor* monitor, int* xpos, int* ypos, int *width, int *height);
    GLFWvidmode* (*_glfwPlatformGetVideoModesFunc)(_GLFWmonitor* monitor, int* count);
    void (*_glfwPlatformGetVideoModeFunc)(_GLFWmonitor* monitor, GLFWvidmode* mode);
    GLFWbool (*_glfwPlatformGetGammaRampFunc)(_GLFWmonitor* monitor, GLFWgammaramp* ramp);
    void (*_glfwPlatformSetGammaRampFunc)(_GLFWmonitor* monitor, const GLFWgammaramp* ramp);

    void (*_glfwPlatformSetClipboardStringFunc)(const char* string);
    const char* (*_glfwPlatformGetClipboardStringFunc)(void);

    int (*_glfwPlatformCreateWindowFunc)(_GLFWwindow* window,
                                         const _GLFWwndconfig* wndconfig,
                                         const _GLFWctxconfig* ctxconfig,
                                         const _GLFWfbconfig* fbconfig);
    void (*_glfwPlatformDestroyWindowFunc)(_GLFWwindow* window);
    void (*_glfwPlatformSetWindowTitleFunc)(_GLFWwindow* window, const char* title);
    void (*_glfwPlatformSetWindowIconFunc)(_GLFWwindow* window,
                                           int count, const GLFWimage* images);
    void (*_glfwPlatformGetWindowPosFunc)(_GLFWwindow* window, int* xpos, int* ypos);
    void (*_glfwPlatformSetWindowPosFunc)(_GLFWwindow* window, int xpos, int ypos);
    void (*_glfwPlatformGetWindowSizeFunc)(_GLFWwindow* window, int* width, int* height);
    void (*_glfwPlatformSetWindowSizeFunc)(_GLFWwindow* window, int width, int height);
    void (*_glfwPlatformSetWindowSizeLimitsFunc)(_GLFWwindow* window,
                                                 int minwidth, int minheight,
                                                 int maxwidth, int maxheight);
    void (*_glfwPlatformSetWindowAspectRatioFunc)(_GLFWwindow* window, int numer, int denom);
    void (*_glfwPlatformGetFramebufferSizeFunc)(_GLFWwindow* window, int* width, int* height);
    void (*_glfwPlatformGetWindowFrameSizeFunc)(_GLFWwindow* window,
                                                int* left, int* top,
                                                int* right, int* bottom);
    void (*_glfwPlatformGetWindowContentScaleFunc)(_GLFWwindow* window,
                                                   float* xscale, float* yscale);
    void (*_glfwPlatformIconifyWindowFunc)(_GLFWwindow* window);
    void (*_glfwPlatformRestoreWindowFunc)(_GLFWwindow* window);
    void (*_glfwPlatformMaximizeWindowFunc)(_GLFWwindow* window);
    void (*_glfwPlatformShowWindowFunc)(_GLFWwindow* window);
    void (*_glfwPlatformHideWindowFunc)(_GLFWwindow* window);
    void (*_glfwPlatformRequestWindowAttentionFunc)(_GLFWwindow* window);
    void (*_glfwPlatformFocusWindowFunc)(_GLFWwindow* window);
    void (*_glfwPlatformSetWindowMonitorFunc)(_GLFWwindow* window, _GLFWmonitor* monitor,
                                              int xpos, int ypos, int width, int height,
                                              int refreshRate);
    int (*_glfwPlatformWindowFocusedFunc)(_GLFWwindow* window);
    int (*_glfwPlatformWindowIconifiedFunc)(_GLFWwindow* window);
    int (*_glfwPlatformWindowVisibleFunc)(_GLFWwindow* window);
    int (*_glfwPlatformWindowMaximizedFunc)(_GLFWwindow* window);
    int (*_glfwPlatformWindowHoveredFunc)(_GLFWwindow* window);
    int (*_glfwPlatformFramebufferTransparentFunc)(_GLFWwindow* window);
    float (*_glfwPlatformGetWindowOpacityFunc)(_GLFWwindow* window);
    void (*_glfwPlatformSetWindowResizableFunc)(_GLFWwindow* window, GLFWbool enabled);
    void (*_glfwPlatformSetWindowDecoratedFunc)(_GLFWwindow* window, GLFWbool enabled);
    void (*_glfwPlatformSetWindowFloatingFunc)(_GLFWwindow* window, GLFWbool enabled);
    void (*_glfwPlatformSetWindowOpacityFunc)(_GLFWwindow* window, float opacity);

    void (*_glfwPlatformPollEventsFunc)(void);
    void (*_glfwPlatformWaitEventsFunc)(void);
    void (*_glfwPlatformWaitEventsTimeoutFunc)(double timeout);
    void (*_glfwPlatformPostEmptyEventFunc)(void);

    void (*_glfwPlatformGetRequiredInstanceExtensionsFunc)(char** extensions);
    int (*_glfwPlatformGetPhysicalDevicePresentationSupportFunc)(VkInstance instance,
                                                                 VkPhysicalDevice device,
                                                                 uint32_t queuefamily);
    VkResult (*_glfwPlatformCreateWindowSurfaceFunc)(VkInstance instance,
                                                     _GLFWwindow* window,
                                                     const VkAllocationCallbacks* allocator,
                                                     VkSurfaceKHR* surface);

    GLFWbool (*_glfwPlatformCreateTlsFunc)(_GLFWtls* tls);
    void (*_glfwPlatformDestroyTlsFunc)(_GLFWtls* tls);
    void* (*_glfwPlatformGetTlsFunc)(_GLFWtls* tls);
    void (*_glfwPlatformSetTlsFunc)(_GLFWtls* tls, void* value);

    GLFWbool (*_glfwPlatformCreateMutexFunc)(_GLFWmutex* mutex);
    void (*_glfwPlatformDestroyMutexFunc)(_GLFWmutex* mutex);
    void (*_glfwPlatformLockMutexFunc)(_GLFWmutex* mutex);
    void (*_glfwPlatformUnlockMutexFunc)(_GLFWmutex* mutex);

    uint64_t (*_glfwPlatformGetTimerValueFunc)(void);
    uint64_t (*_glfwPlatformGetTimerFrequencyFunc)(void);

    int (*_glfwPlatformPollJoystickFunc)(_GLFWjoystick* js, int mode);
    void (*_glfwPlatformUpdateGamepadGUIDFunc)(char* guid);
} _GLFWplatformfunctions;


extern GLFWbool _glfwPlatformLoadFunctions(_GLFWplatformfunctions);

//////////////////////////////////////////////////////////////////////////
//////                       GLFW platform API                      //////
//////////////////////////////////////////////////////////////////////////

extern int  (*_glfwPlatformInit)(void);
extern void (*_glfwPlatformTerminate)(void);
extern const char* (*_glfwPlatformGetVersionString)(void);

extern void (*_glfwPlatformGetCursorPos)(_GLFWwindow* window, double* xpos, double* ypos);
extern void (*_glfwPlatformSetCursorPos)(_GLFWwindow* window, double xpos, double ypos);
extern void (*_glfwPlatformSetCursorMode)(_GLFWwindow* window, int mode);
extern void (*_glfwPlatformSetRawMouseMotion)(_GLFWwindow *window, GLFWbool enabled);
extern GLFWbool (*_glfwPlatformRawMouseMotionSupported)(void);
extern int (*_glfwPlatformCreateCursor)(_GLFWcursor* cursor,
                                        const GLFWimage* image, int xhot, int yhot);
extern int (*_glfwPlatformCreateStandardCursor)(_GLFWcursor* cursor, int shape);
extern void (*_glfwPlatformDestroyCursor)(_GLFWcursor* cursor);
extern void (*_glfwPlatformSetCursor)(_GLFWwindow* window, _GLFWcursor* cursor);

extern const char* (*_glfwPlatformGetScancodeName)(int scancode);
extern int (*_glfwPlatformGetKeyScancode)(int key);

extern void (*_glfwPlatformFreeMonitor)(_GLFWmonitor* monitor);
extern void (*_glfwPlatformGetMonitorPos)(_GLFWmonitor* monitor, int* xpos, int* ypos);
extern void (*_glfwPlatformGetMonitorContentScale)(_GLFWmonitor* monitor,
                                                   float* xscale, float* yscale);
extern void (*_glfwPlatformGetMonitorWorkarea)(_GLFWmonitor* monitor, int* xpos, int* ypos, int *width, int *height);
extern GLFWvidmode* (*_glfwPlatformGetVideoModes)(_GLFWmonitor* monitor, int* count);
extern void (*_glfwPlatformGetVideoMode)(_GLFWmonitor* monitor, GLFWvidmode* mode);
extern GLFWbool (*_glfwPlatformGetGammaRamp)(_GLFWmonitor* monitor, GLFWgammaramp* ramp);
extern void (*_glfwPlatformSetGammaRamp)(_GLFWmonitor* monitor, const GLFWgammaramp* ramp);

extern void (*_glfwPlatformSetClipboardString)(const char* string);
extern const char* (*_glfwPlatformGetClipboardString)(void);

extern int (*_glfwPlatformPollJoystick)(_GLFWjoystick* js, int mode);
extern void (*_glfwPlatformUpdateGamepadGUID)(char* guid);

extern uint64_t (*_glfwPlatformGetTimerValue)(void);
extern uint64_t (*_glfwPlatformGetTimerFrequency)(void);

extern int (*_glfwPlatformCreateWindow)(_GLFWwindow* window,
                                        const _GLFWwndconfig* wndconfig,
                                        const _GLFWctxconfig* ctxconfig,
                                        const _GLFWfbconfig* fbconfig);
extern void (*_glfwPlatformDestroyWindow)(_GLFWwindow* window);
extern void (*_glfwPlatformSetWindowTitle)(_GLFWwindow* window, const char* title);
extern void (*_glfwPlatformSetWindowIcon)(_GLFWwindow* window,
                                          int count, const GLFWimage* images);
extern void (*_glfwPlatformGetWindowPos)(_GLFWwindow* window, int* xpos, int* ypos);
extern void (*_glfwPlatformSetWindowPos)(_GLFWwindow* window, int xpos, int ypos);
extern void (*_glfwPlatformGetWindowSize)(_GLFWwindow* window, int* width, int* height);
extern void (*_glfwPlatformSetWindowSize)(_GLFWwindow* window, int width, int height);
extern void (*_glfwPlatformSetWindowSizeLimits)(_GLFWwindow* window,
                                                int minwidth, int minheight,
                                                int maxwidth, int maxheight);
extern void (*_glfwPlatformSetWindowAspectRatio)(_GLFWwindow* window, int numer, int denom);
extern void (*_glfwPlatformGetFramebufferSize)(_GLFWwindow* window, int* width, int* height);
extern void (*_glfwPlatformGetWindowFrameSize)(_GLFWwindow* window,
                                               int* left, int* top,
                                               int* right, int* bottom);
extern void (*_glfwPlatformGetWindowContentScale)(_GLFWwindow* window,
                                                  float* xscale, float* yscale);
extern void (*_glfwPlatformIconifyWindow)(_GLFWwindow* window);
extern void (*_glfwPlatformRestoreWindow)(_GLFWwindow* window);
extern void (*_glfwPlatformMaximizeWindow)(_GLFWwindow* window);
extern void (*_glfwPlatformShowWindow)(_GLFWwindow* window);
extern void (*_glfwPlatformHideWindow)(_GLFWwindow* window);
extern void (*_glfwPlatformRequestWindowAttention)(_GLFWwindow* window);
extern void (*_glfwPlatformFocusWindow)(_GLFWwindow* window);
extern void (*_glfwPlatformSetWindowMonitor)(_GLFWwindow* window, _GLFWmonitor* monitor,
                                             int xpos, int ypos, int width, int height,
                                             int refreshRate);
extern int (*_glfwPlatformWindowFocused)(_GLFWwindow* window);
extern int (*_glfwPlatformWindowIconified)(_GLFWwindow* window);
extern int (*_glfwPlatformWindowVisible)(_GLFWwindow* window);
extern int (*_glfwPlatformWindowMaximized)(_GLFWwindow* window);
extern int (*_glfwPlatformWindowHovered)(_GLFWwindow* window);
extern int (*_glfwPlatformFramebufferTransparent)(_GLFWwindow* window);
extern float (*_glfwPlatformGetWindowOpacity)(_GLFWwindow* window);
extern void (*_glfwPlatformSetWindowResizable)(_GLFWwindow* window, GLFWbool enabled);
extern void (*_glfwPlatformSetWindowDecorated)(_GLFWwindow* window, GLFWbool enabled);
extern void (*_glfwPlatformSetWindowFloating)(_GLFWwindow* window, GLFWbool enabled);
extern void (*_glfwPlatformSetWindowOpacity)(_GLFWwindow* window, float opacity);

extern void (*_glfwPlatformPollEvents)(void);
extern void (*_glfwPlatformWaitEvents)(void);
extern void (*_glfwPlatformWaitEventsTimeout)(double timeout);
extern void (*_glfwPlatformPostEmptyEvent)(void);

extern void (*_glfwPlatformGetRequiredInstanceExtensions)(char** extensions);
extern int (*_glfwPlatformGetPhysicalDevicePresentationSupport)(VkInstance instance,
                                                                VkPhysicalDevice device,
                                                                uint32_t queuefamily);
extern VkResult (*_glfwPlatformCreateWindowSurface)(VkInstance instance,
                                                    _GLFWwindow* window,
                                                    const VkAllocationCallbacks* allocator,
                                                    VkSurfaceKHR* surface);

extern GLFWbool (*_glfwPlatformCreateTls)(_GLFWtls* tls);
extern void (*_glfwPlatformDestroyTls)(_GLFWtls* tls);
extern void* (*_glfwPlatformGetTls)(_GLFWtls* tls);
extern void (*_glfwPlatformSetTls)(_GLFWtls* tls, void* value);

extern GLFWbool (*_glfwPlatformCreateMutex)(_GLFWmutex* mutex);
extern void (*_glfwPlatformDestroyMutex)(_GLFWmutex* mutex);
extern void (*_glfwPlatformLockMutex)(_GLFWmutex* mutex);
extern void (*_glfwPlatformUnlockMutex)(_GLFWmutex* mutex);

extern int (*_glfwPlatformPollJoystick)(_GLFWjoystick* js, int mode);
extern void (*_glfwPlatformUpdateGamepadGUID)(char* guid);
