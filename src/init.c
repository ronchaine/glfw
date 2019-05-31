//========================================================================
// GLFW 3.4 - www.glfw.org
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2018 Camilla Löwy <elmindreda@glfw.org>
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
// Please use C89 style variable declarations in this file because VS 2010
//========================================================================

#include "internal.h"
#include "mappings.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>


// The global variables below comprise all mutable global data in GLFW
//
// Any other global variable is a bug

// Global state shared between compilation units of GLFW
//
_GLFWlibrary _glfw = { GLFW_FALSE };

// These are outside of _glfw so they can be used before initialization and
// after termination
//
static _GLFWerror _glfwMainThreadError;
static GLFWerrorfun _glfwErrorCallback;
static _GLFWinitconfig _glfwInitHints =
{
    GLFW_TRUE,      // hat buttons
    {
        GLFW_TRUE,  // macOS menu bar
        GLFW_TRUE   // macOS bundle chdir
    }
};

// Declare GLFW platform functions in this file
int  (*_glfwPlatformInit)(void);
void (*_glfwPlatformTerminate)(void);
const char* (*_glfwPlatformGetVersionString)(void);

void (*_glfwPlatformGetCursorPos)(_GLFWwindow* window, double* xpos, double* ypos);
void (*_glfwPlatformSetCursorPos)(_GLFWwindow* window, double xpos, double ypos);
void (*_glfwPlatformSetCursorMode)(_GLFWwindow* window, int mode);
void (*_glfwPlatformSetRawMouseMotion)(_GLFWwindow *window, GLFWbool enabled);
GLFWbool (*_glfwPlatformRawMouseMotionSupported)(void);
int (*_glfwPlatformCreateCursor)(_GLFWcursor* cursor,
                                 const GLFWimage* image, int xhot, int yhot);
int (*_glfwPlatformCreateStandardCursor)(_GLFWcursor* cursor, int shape);
void (*_glfwPlatformDestroyCursor)(_GLFWcursor* cursor);
void (*_glfwPlatformSetCursor)(_GLFWwindow* window, _GLFWcursor* cursor);

const char* (*_glfwPlatformGetScancodeName)(int scancode);
int (*_glfwPlatformGetKeyScancode)(int key);

void (*_glfwPlatformFreeMonitor)(_GLFWmonitor* monitor);
void (*_glfwPlatformGetMonitorPos)(_GLFWmonitor* monitor, int* xpos, int* ypos);
void (*_glfwPlatformGetMonitorContentScale)(_GLFWmonitor* monitor,
                                            float* xscale, float* yscale);
void (*_glfwPlatformGetMonitorWorkarea)(_GLFWmonitor* monitor, int* xpos, int* ypos, int *width, int *height);
GLFWvidmode* (*_glfwPlatformGetVideoModes)(_GLFWmonitor* monitor, int* count);
void (*_glfwPlatformGetVideoMode)(_GLFWmonitor* monitor, GLFWvidmode* mode);
GLFWbool (*_glfwPlatformGetGammaRamp)(_GLFWmonitor* monitor, GLFWgammaramp* ramp);
void (*_glfwPlatformSetGammaRamp)(_GLFWmonitor* monitor, const GLFWgammaramp* ramp);

void (*_glfwPlatformSetClipboardString)(const char* string);
const char* (*_glfwPlatformGetClipboardString)(void);

int (*_glfwPlatformPollJoystick)(_GLFWjoystick* js, int mode);
void (*_glfwPlatformUpdateGamepadGUID)(char* guid);

uint64_t (*_glfwPlatformGetTimerValue)(void);
uint64_t (*_glfwPlatformGetTimerFrequency)(void);

int (*_glfwPlatformCreateWindow)(_GLFWwindow* window,
                                 const _GLFWwndconfig* wndconfig,
                                 const _GLFWctxconfig* ctxconfig,
                                 const _GLFWfbconfig* fbconfig);
void (*_glfwPlatformDestroyWindow)(_GLFWwindow* window);
void (*_glfwPlatformSetWindowTitle)(_GLFWwindow* window, const char* title);
void (*_glfwPlatformSetWindowIcon)(_GLFWwindow* window,
                                   int count, const GLFWimage* images);
void (*_glfwPlatformGetWindowPos)(_GLFWwindow* window, int* xpos, int* ypos);
void (*_glfwPlatformSetWindowPos)(_GLFWwindow* window, int xpos, int ypos);
void (*_glfwPlatformGetWindowSize)(_GLFWwindow* window, int* width, int* height);
void (*_glfwPlatformSetWindowSize)(_GLFWwindow* window, int width, int height);
void (*_glfwPlatformSetWindowSizeLimits)(_GLFWwindow* window,
                                         int minwidth, int minheight,
                                         int maxwidth, int maxheight);
void (*_glfwPlatformSetWindowAspectRatio)(_GLFWwindow* window, int numer, int denom);
void (*_glfwPlatformGetFramebufferSize)(_GLFWwindow* window, int* width, int* height);
void (*_glfwPlatformGetWindowFrameSize)(_GLFWwindow* window,
                                        int* left, int* top,
                                        int* right, int* bottom);
void (*_glfwPlatformGetWindowContentScale)(_GLFWwindow* window,
                                           float* xscale, float* yscale);
void (*_glfwPlatformIconifyWindow)(_GLFWwindow* window);
void (*_glfwPlatformRestoreWindow)(_GLFWwindow* window);
void (*_glfwPlatformMaximizeWindow)(_GLFWwindow* window);
void (*_glfwPlatformShowWindow)(_GLFWwindow* window);
void (*_glfwPlatformHideWindow)(_GLFWwindow* window);
void (*_glfwPlatformRequestWindowAttention)(_GLFWwindow* window);
void (*_glfwPlatformFocusWindow)(_GLFWwindow* window);
void (*_glfwPlatformSetWindowMonitor)(_GLFWwindow* window, _GLFWmonitor* monitor,
                                      int xpos, int ypos, int width, int height,
                                      int refreshRate);
int (*_glfwPlatformWindowFocused)(_GLFWwindow* window);
int (*_glfwPlatformWindowIconified)(_GLFWwindow* window);
int (*_glfwPlatformWindowVisible)(_GLFWwindow* window);
int (*_glfwPlatformWindowMaximized)(_GLFWwindow* window);
int (*_glfwPlatformWindowHovered)(_GLFWwindow* window);
int (*_glfwPlatformFramebufferTransparent)(_GLFWwindow* window);
float (*_glfwPlatformGetWindowOpacity)(_GLFWwindow* window);
void (*_glfwPlatformSetWindowResizable)(_GLFWwindow* window, GLFWbool enabled);
void (*_glfwPlatformSetWindowDecorated)(_GLFWwindow* window, GLFWbool enabled);
void (*_glfwPlatformSetWindowFloating)(_GLFWwindow* window, GLFWbool enabled);
void (*_glfwPlatformSetWindowOpacity)(_GLFWwindow* window, float opacity);

void (*_glfwPlatformPollEvents)(void);
void (*_glfwPlatformWaitEvents)(void);
void (*_glfwPlatformWaitEventsTimeout)(double timeout);
void (*_glfwPlatformPostEmptyEvent)(void);

void (*_glfwPlatformGetRequiredInstanceExtensions)(char** extensions);
int (*_glfwPlatformGetPhysicalDevicePresentationSupport)(VkInstance instance,
                                                         VkPhysicalDevice device,
                                                         uint32_t queuefamily);
VkResult (*_glfwPlatformCreateWindowSurface)(VkInstance instance,
                                             _GLFWwindow* window,
                                             const VkAllocationCallbacks* allocator,
                                             VkSurfaceKHR* surface);

GLFWbool (*_glfwPlatformCreateTls)(_GLFWtls* tls);
void (*_glfwPlatformDestroyTls)(_GLFWtls* tls);
void* (*_glfwPlatformGetTls)(_GLFWtls* tls);
void (*_glfwPlatformSetTls)(_GLFWtls* tls, void* value);

GLFWbool (*_glfwPlatformCreateMutex)(_GLFWmutex* mutex);
void (*_glfwPlatformDestroyMutex)(_GLFWmutex* mutex);
void (*_glfwPlatformLockMutex)(_GLFWmutex* mutex);
void (*_glfwPlatformUnlockMutex)(_GLFWmutex* mutex);

// Terminate the library
//
static void terminate(void)
{
    int i;

    memset(&_glfw.callbacks, 0, sizeof(_glfw.callbacks));

    while (_glfw.windowListHead)
        glfwDestroyWindow((GLFWwindow*) _glfw.windowListHead);

    while (_glfw.cursorListHead)
        glfwDestroyCursor((GLFWcursor*) _glfw.cursorListHead);

    for (i = 0;  i < _glfw.monitorCount;  i++)
    {
        _GLFWmonitor* monitor = _glfw.monitors[i];
        if (monitor->originalRamp.size)
            _glfwPlatformSetGammaRamp(monitor, &monitor->originalRamp);
        _glfwFreeMonitor(monitor);
    }

    free(_glfw.monitors);
    _glfw.monitors = NULL;
    _glfw.monitorCount = 0;

    free(_glfw.mappings);
    _glfw.mappings = NULL;
    _glfw.mappingCount = 0;

    _glfwTerminateVulkan();
    _glfwPlatformTerminate();

    _glfw.initialized = GLFW_FALSE;

    while (_glfw.errorListHead)
    {
        _GLFWerror* error = _glfw.errorListHead;
        _glfw.errorListHead = error->next;
        free(error);
    }

    _glfwPlatformDestroyTls(&_glfw.contextSlot);
    _glfwPlatformDestroyTls(&_glfw.errorSlot);
    _glfwPlatformDestroyMutex(&_glfw.errorLock);

    memset(&_glfw, 0, sizeof(_glfw));
}

GLFWbool _glfwPlatformLoadFunctions(_GLFWplatformfunctions funcs)
{
    _glfwPlatformInit = funcs._glfwPlatformInitFunc;
    _glfwPlatformTerminate = funcs._glfwPlatformTerminateFunc;
    _glfwPlatformGetVersionString = funcs._glfwPlatformGetVersionStringFunc;

    _glfwPlatformFreeMonitor = funcs._glfwPlatformFreeMonitorFunc;
    _glfwPlatformGetMonitorPos = funcs._glfwPlatformGetMonitorPosFunc;
    _glfwPlatformGetMonitorContentScale = funcs._glfwPlatformGetMonitorContentScaleFunc;
    _glfwPlatformGetMonitorWorkarea = funcs._glfwPlatformGetMonitorWorkareaFunc;
    _glfwPlatformGetVideoModes = funcs._glfwPlatformGetVideoModesFunc;
    _glfwPlatformGetVideoMode = funcs._glfwPlatformGetVideoModeFunc;
    _glfwPlatformGetGammaRamp = funcs._glfwPlatformGetGammaRampFunc;
    _glfwPlatformSetGammaRamp = funcs._glfwPlatformSetGammaRampFunc;

    _glfwPlatformGetCursorPos = funcs._glfwPlatformGetCursorPosFunc;
    _glfwPlatformSetCursorPos = funcs._glfwPlatformSetCursorPosFunc;
    _glfwPlatformSetCursorMode = funcs._glfwPlatformSetCursorModeFunc;
    _glfwPlatformSetRawMouseMotion = funcs._glfwPlatformSetRawMouseMotionFunc;
    _glfwPlatformRawMouseMotionSupported = funcs._glfwPlatformRawMouseMotionSupportedFunc;
    _glfwPlatformCreateCursor = funcs._glfwPlatformCreateCursorFunc;
    _glfwPlatformCreateStandardCursor = funcs._glfwPlatformCreateStandardCursorFunc;
    _glfwPlatformDestroyCursor = funcs._glfwPlatformDestroyCursorFunc;
    _glfwPlatformSetCursor = funcs._glfwPlatformSetCursorFunc;

    _glfwPlatformGetScancodeName = funcs._glfwPlatformGetScancodeNameFunc;
    _glfwPlatformGetKeyScancode = funcs._glfwPlatformGetKeyScancodeFunc;

    _glfwPlatformSetClipboardString = funcs._glfwPlatformSetClipboardStringFunc;
    _glfwPlatformGetClipboardString = funcs._glfwPlatformGetClipboardStringFunc;

    _glfwPlatformPollJoystick = funcs._glfwPlatformPollJoystickFunc;
    _glfwPlatformUpdateGamepadGUID = funcs._glfwPlatformUpdateGamepadGUIDFunc;

    _glfwPlatformGetTimerValue = funcs._glfwPlatformGetTimerValueFunc;
    _glfwPlatformGetTimerFrequency = funcs._glfwPlatformGetTimerFrequencyFunc;

    _glfwPlatformCreateWindow = funcs._glfwPlatformCreateWindowFunc;
    _glfwPlatformDestroyWindow = funcs._glfwPlatformDestroyWindowFunc;
    _glfwPlatformSetWindowTitle = funcs._glfwPlatformSetWindowTitleFunc;
    _glfwPlatformSetWindowIcon = funcs._glfwPlatformSetWindowIconFunc;
    _glfwPlatformGetWindowPos = funcs._glfwPlatformGetWindowPosFunc;
    _glfwPlatformSetWindowPos = funcs._glfwPlatformSetWindowPosFunc;
    _glfwPlatformGetWindowSize = funcs._glfwPlatformGetWindowSizeFunc;
    _glfwPlatformSetWindowSize = funcs._glfwPlatformSetWindowSizeFunc;
    _glfwPlatformSetWindowSizeLimits = funcs._glfwPlatformSetWindowSizeLimitsFunc;
    _glfwPlatformSetWindowAspectRatio = funcs._glfwPlatformSetWindowAspectRatioFunc;
    _glfwPlatformGetFramebufferSize = funcs._glfwPlatformGetFramebufferSizeFunc;
    _glfwPlatformGetWindowFrameSize = funcs._glfwPlatformGetWindowFrameSizeFunc;
    _glfwPlatformGetWindowContentScale = funcs._glfwPlatformGetWindowContentScaleFunc;
    _glfwPlatformIconifyWindow = funcs._glfwPlatformIconifyWindowFunc;
    _glfwPlatformRestoreWindow = funcs._glfwPlatformRestoreWindowFunc;
    _glfwPlatformMaximizeWindow = funcs._glfwPlatformMaximizeWindowFunc;
    _glfwPlatformShowWindow = funcs._glfwPlatformShowWindowFunc;
    _glfwPlatformHideWindow = funcs._glfwPlatformHideWindowFunc;
    _glfwPlatformRequestWindowAttention = funcs._glfwPlatformRequestWindowAttentionFunc;
    _glfwPlatformFocusWindow = funcs._glfwPlatformFocusWindowFunc;
    _glfwPlatformSetWindowMonitor = funcs._glfwPlatformSetWindowMonitorFunc;
    _glfwPlatformWindowFocused = funcs._glfwPlatformWindowFocusedFunc;
    _glfwPlatformWindowIconified = funcs._glfwPlatformWindowIconifiedFunc;
    _glfwPlatformWindowVisible = funcs._glfwPlatformWindowVisibleFunc;
    _glfwPlatformWindowMaximized = funcs._glfwPlatformWindowMaximizedFunc;
    _glfwPlatformWindowHovered = funcs._glfwPlatformWindowHoveredFunc;
    _glfwPlatformFramebufferTransparent = funcs._glfwPlatformFramebufferTransparentFunc;
    _glfwPlatformGetWindowOpacity = funcs._glfwPlatformGetWindowOpacityFunc;
    _glfwPlatformSetWindowResizable = funcs._glfwPlatformSetWindowResizableFunc;
    _glfwPlatformSetWindowDecorated = funcs._glfwPlatformSetWindowDecoratedFunc;
    _glfwPlatformSetWindowFloating = funcs._glfwPlatformSetWindowFloatingFunc;
    _glfwPlatformSetWindowOpacity = funcs._glfwPlatformSetWindowOpacityFunc;

    _glfwPlatformPollEvents = funcs._glfwPlatformPollEventsFunc;
    _glfwPlatformWaitEvents = funcs._glfwPlatformWaitEventsFunc;
    _glfwPlatformWaitEventsTimeout = funcs._glfwPlatformWaitEventsTimeoutFunc;
    _glfwPlatformPostEmptyEvent = funcs._glfwPlatformPostEmptyEventFunc;

    _glfwPlatformGetRequiredInstanceExtensions = funcs._glfwPlatformGetRequiredInstanceExtensionsFunc;
    _glfwPlatformGetPhysicalDevicePresentationSupport = funcs._glfwPlatformGetPhysicalDevicePresentationSupportFunc;
    _glfwPlatformCreateWindowSurface = funcs._glfwPlatformCreateWindowSurfaceFunc;

    _glfwPlatformCreateTls = funcs._glfwPlatformCreateTlsFunc;
    _glfwPlatformDestroyTls = funcs._glfwPlatformDestroyTlsFunc;
    _glfwPlatformGetTls = funcs._glfwPlatformGetTlsFunc;
    _glfwPlatformSetTls = funcs._glfwPlatformSetTlsFunc;

    _glfwPlatformCreateMutex = funcs._glfwPlatformCreateMutexFunc;
    _glfwPlatformDestroyMutex = funcs._glfwPlatformDestroyMutexFunc;
    _glfwPlatformLockMutex = funcs._glfwPlatformLockMutexFunc;
    _glfwPlatformUnlockMutex = funcs._glfwPlatformUnlockMutexFunc;

    _glfwPlatformGetTimerValue = funcs._glfwPlatformGetTimerValueFunc;
    _glfwPlatformGetTimerFrequency = funcs._glfwPlatformGetTimerFrequencyFunc;

    _glfwPlatformPollJoystick = funcs._glfwPlatformPollJoystickFunc;
    _glfwPlatformUpdateGamepadGUID = funcs._glfwPlatformUpdateGamepadGUIDFunc;
    /*
    if (_glfwPlatformInit == NULL)
    if (_glfwPlatformTerminate == NULL)
    if (_glfwPlatformGetVersionString == NULL)

    if (_glfwPlatformFreeMonitor == NULL)
    if (_glfwPlatformGetMonitorPos == NULL)
    if (_glfwPlatformGetMonitorContentScale == NULL)
    if (_glfwPlatformGetMonitorWorkarea == NULL)
    if (_glfwPlatformGetVideoModes == NULL)
    if (_glfwPlatformGetVideoMode == NULL)
    if (_glfwPlatformGetGammaRamp == NULL)
    if (_glfwPlatformSetGammaRamp == NULL)

    if (_glfwPlatformGetCursorPos == NULL)
    if (_glfwPlatformSetCursorPos == NULL)
    if (_glfwPlatformSetCursorMode == NULL)
    if (_glfwPlatformSetRawMouseMotion == NULL)
    if (_glfwPlatformRawMouseMotionSupported == NULL)
    if (_glfwPlatformCreateCursor == NULL)
    if (_glfwPlatformCreateStandardCursor == NULL)
    if (_glfwPlatformDestroyCursor == NULL)
    if (_glfwPlatformSetCursor == NULL)

    if (_glfwPlatformGetScancodeName == NULL)
    if (_glfwPlatformGetKeyScancode == NULL)

    if (_glfwPlatformSetClipboardString == NULL)
    if (_glfwPlatformGetClipboardString == NULL)

    if (_glfwPlatformPollJoystick == NULL)
    if (_glfwPlatformUpdateGamepadGUID == NULL)

    if (_glfwPlatformGetTimerValue == NULL)
    if (_glfwPlatformGetTimerFrequency == NULL)

    if (_glfwPlatformCreateWindow == NULL)
    if (_glfwPlatformDestroyWindow == NULL)
    if (_glfwPlatformSetWindowTitle == NULL)
    if (_glfwPlatformSetWindowIcon == NULL)
    if (_glfwPlatformGetWindowPos == NULL)
    if (_glfwPlatformSetWindowPos == NULL)
    if (_glfwPlatformGetWindowSize == NULL)
    if (_glfwPlatformSetWindowSize == NULL)
    if (_glfwPlatformSetWindowSizeLimits == NULL)
    if (_glfwPlatformSetWindowAspectRatio == NULL)
    if (_glfwPlatformGetFramebufferSize = funcs._glfwPlatformGetFramebufferSizeFunc;
    if (_glfwPlatformGetWindowFrameSize = funcs._glfwPlatformGetWindowFrameSizeFunc;
    if (_glfwPlatformGetWindowContentScale = funcs._glfwPlatformGetWindowContentScaleFunc;
    if (_glfwPlatformIconifyWindow = funcs._glfwPlatformIconifyWindowFunc;
    if (_glfwPlatformRestoreWindow = funcs._glfwPlatformRestoreWindowFunc;
    if (_glfwPlatformMaximizeWindow = funcs._glfwPlatformMaximizeWindowFunc;
    if (_glfwPlatformShowWindow = funcs._glfwPlatformShowWindowFunc;
    if (_glfwPlatformHideWindow = funcs._glfwPlatformHideWindowFunc;
    if (_glfwPlatformRequestWindowAttention = funcs._glfwPlatformRequestWindowAttentionFunc;
    if (_glfwPlatformFocusWindow = funcs._glfwPlatformFocusWindowFunc;
    if (_glfwPlatformSetWindowMonitor = funcs._glfwPlatformSetWindowMonitorFunc;
    if (_glfwPlatformWindowFocused = funcs._glfwPlatformWindowFocusedFunc;
    if (_glfwPlatformWindowIconified = funcs._glfwPlatformWindowIconifiedFunc;
    if (_glfwPlatformWindowVisible = funcs._glfwPlatformWindowVisibleFunc;
    if (_glfwPlatformWindowMaximized = funcs._glfwPlatformWindowMaximizedFunc;
    if (_glfwPlatformWindowHovered = funcs._glfwPlatformWindowHoveredFunc;
    if (_glfwPlatformFramebufferTransparent = funcs._glfwPlatformFramebufferTransparentFunc;
    if (_glfwPlatformGetWindowOpacity = funcs._glfwPlatformGetWindowOpacityFunc;
    if (_glfwPlatformSetWindowResizable = funcs._glfwPlatformSetWindowResizableFunc;
    if (_glfwPlatformSetWindowDecorated = funcs._glfwPlatformSetWindowDecoratedFunc;
    if (_glfwPlatformSetWindowFloating = funcs._glfwPlatformSetWindowFloatingFunc;
    if (_glfwPlatformSetWindowOpacity = funcs._glfwPlatformSetWindowOpacityFunc;

    if (_glfwPlatformPollEvents = funcs._glfwPlatformPollEventsFunc;
    if (_glfwPlatformWaitEvents = funcs._glfwPlatformWaitEventsFunc;
    if (_glfwPlatformWaitEventsTimeout = funcs._glfwPlatformWaitEventsTimeoutFunc;
    if (_glfwPlatformPostEmptyEvent = funcs._glfwPlatformPostEmptyEventFunc;

    if (_glfwPlatformGetRequiredInstanceExtensions = funcs._glfwPlatformGetRequiredInstanceExtensionsFunc;
    if (_glfwPlatformGetPhysicalDevicePresentationSupport = funcs._glfwPlatformGetPhysicalDevicePresentationSupportFunc;
    if (_glfwPlatformCreateWindowSurface = funcs._glfwPlatformCreateWindowSurfaceFunc;

    if (_glfwPlatformCreateTls = funcs._glfwPlatformCreateTlsFunc;
    if (_glfwPlatformDestroyTls = funcs._glfwPlatformDestroyTlsFunc;
    if (_glfwPlatformGetTls = funcs._glfwPlatformGetTlsFunc;
    if (_glfwPlatformSetTls = funcs._glfwPlatformSetTlsFunc;

    if (_glfwPlatformCreateMutex = funcs._glfwPlatformCreateMutexFunc;
    if (_glfwPlatformDestroyMutex = funcs._glfwPlatformDestroyMutexFunc;
    if (_glfwPlatformLockMutex = funcs._glfwPlatformLockMutexFunc;
    if (_glfwPlatformUnlockMutex = funcs._glfwPlatformUnlockMutexFunc;
    */
    return GLFW_TRUE;
}

//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

char* _glfw_strdup(const char* source)
{
    const size_t length = strlen(source);
    char* result = calloc(length + 1, 1);
    strcpy(result, source);
    return result;
}

float _glfw_fminf(float a, float b)
{
    if (a != a)
        return b;
    else if (b != b)
        return a;
    else if (a < b)
        return a;
    else
        return b;
}

float _glfw_fmaxf(float a, float b)
{
    if (a != a)
        return b;
    else if (b != b)
        return a;
    else if (a > b)
        return a;
    else
        return b;
}


//////////////////////////////////////////////////////////////////////////
//////                         GLFW event API                       //////
//////////////////////////////////////////////////////////////////////////

// Notifies shared code of an error
//
void _glfwInputError(int code, const char* format, ...)
{
    _GLFWerror* error;
    char description[_GLFW_MESSAGE_SIZE];

    if (format)
    {
        va_list vl;

        va_start(vl, format);
        vsnprintf(description, sizeof(description), format, vl);
        va_end(vl);

        description[sizeof(description) - 1] = '\0';
    }
    else
    {
        if (code == GLFW_NOT_INITIALIZED)
            strcpy(description, "The GLFW library is not initialized");
        else if (code == GLFW_NO_CURRENT_CONTEXT)
            strcpy(description, "There is no current context");
        else if (code == GLFW_INVALID_ENUM)
            strcpy(description, "Invalid argument for enum parameter");
        else if (code == GLFW_INVALID_VALUE)
            strcpy(description, "Invalid value for parameter");
        else if (code == GLFW_OUT_OF_MEMORY)
            strcpy(description, "Out of memory");
        else if (code == GLFW_API_UNAVAILABLE)
            strcpy(description, "The requested API is unavailable");
        else if (code == GLFW_VERSION_UNAVAILABLE)
            strcpy(description, "The requested API version is unavailable");
        else if (code == GLFW_PLATFORM_ERROR)
            strcpy(description, "A platform-specific error occurred");
        else if (code == GLFW_FORMAT_UNAVAILABLE)
            strcpy(description, "The requested format is unavailable");
        else if (code == GLFW_NO_WINDOW_CONTEXT)
            strcpy(description, "The specified window has no context");
        else if (code == GLFW_CURSOR_UNAVAILABLE)
            strcpy(description, "The specified cursor shape is unavailable");
        else
            strcpy(description, "ERROR: UNKNOWN GLFW ERROR");
    }

    if (_glfw.initialized)
    {
        error = _glfwPlatformGetTls(&_glfw.errorSlot);
        if (!error)
        {
            error = calloc(1, sizeof(_GLFWerror));
            _glfwPlatformSetTls(&_glfw.errorSlot, error);
            _glfwPlatformLockMutex(&_glfw.errorLock);
            error->next = _glfw.errorListHead;
            _glfw.errorListHead = error;
            _glfwPlatformUnlockMutex(&_glfw.errorLock);
        }
    }
    else
        error = &_glfwMainThreadError;

    error->code = code;
    strcpy(error->description, description);

    if (_glfwErrorCallback)
        _glfwErrorCallback(code, description);
}


//////////////////////////////////////////////////////////////////////////
//////                        GLFW public API                       //////
//////////////////////////////////////////////////////////////////////////

GLFWAPI int glfwInit(void)
{
    if (_glfw.initialized)
        return GLFW_TRUE;

    memset(&_glfw, 0, sizeof(_glfw));
    _glfw.hints.init = _glfwInitHints;

    switch(_glfw.backend)
    {
        #if defined(_GLFW_COCOA)
        case GLFW_PLATFORM_COCOA:
            _glfwPlatformLoadFunctions(_glfwFunctionsCocoa);
            break;
        #endif
        #if defined(_GLFW_WIN32)
        case GLFW_PLATFORM_WIN32:
            _glfwPlatformLoadFunctions(_glfwFunctionsWin32);
            break;
        #endif
        #if defined(_GLFW_X11)
        case GLFW_PLATFORM_X11:
            _glfwPlatformLoadFunctions(_glfwFunctionsX11);
            break;
        #endif
        #if defined(_GLFW_WAYLAND)
        case GLFW_PLATFORM_WAYLAND:
            _glfwPlatformLoadFunctions(_glfwFunctionsWayland);
            break;
        #endif
        #if defined(_GLFW_GBM)
        case GLFW_PLATFORM_GBM:
            _glfwPlatformLoadFunctions(_glfwFunctionsGBM);
            break;
        #endif
        default:
            _glfw.backend = _GLFW_DEFAULT_PLATFORM;
            _glfwPlatformLoadFunctions(_GLFW_DEFAULT_PLATFORM_FUNCTIONS);
            break;
    }

    if (!_glfwPlatformInit())
    {
        terminate();
        return GLFW_FALSE;
    }

    if (!_glfwPlatformCreateMutex(&_glfw.errorLock) ||
        !_glfwPlatformCreateTls(&_glfw.errorSlot) ||
        !_glfwPlatformCreateTls(&_glfw.contextSlot))
    {
        terminate();
        return GLFW_FALSE;
    }

    _glfwPlatformSetTls(&_glfw.errorSlot, &_glfwMainThreadError);

    _glfw.initialized = GLFW_TRUE;
    _glfw.timer.offset = _glfwPlatformGetTimerValue();

    glfwDefaultWindowHints();

    {
        int i;

        for (i = 0;  _glfwDefaultMappings[i];  i++)
        {
            if (!glfwUpdateGamepadMappings(_glfwDefaultMappings[i]))
            {
                terminate();
                return GLFW_FALSE;
            }
        }
    }

    return GLFW_TRUE;
}

GLFWAPI void glfwTerminate(void)
{
    if (!_glfw.initialized)
        return;

    terminate();
}

GLFWAPI void glfwInitHint(int hint, int value)
{
    switch (hint)
    {
        case GLFW_JOYSTICK_HAT_BUTTONS:
            _glfwInitHints.hatButtons = value;
            return;
        case GLFW_COCOA_CHDIR_RESOURCES:
            _glfwInitHints.ns.chdir = value;
            return;
        case GLFW_COCOA_MENUBAR:
            _glfwInitHints.ns.menubar = value;
            return;
    }

    _glfwInputError(GLFW_INVALID_ENUM,
                    "Invalid init hint 0x%08X", hint);
}

GLFWAPI void glfwGetVersion(int* major, int* minor, int* rev)
{
    if (major != NULL)
        *major = GLFW_VERSION_MAJOR;
    if (minor != NULL)
        *minor = GLFW_VERSION_MINOR;
    if (rev != NULL)
        *rev = GLFW_VERSION_REVISION;
}

GLFWAPI const char* glfwGetVersionString(void)
{
    return _glfwPlatformGetVersionString();
}

GLFWAPI int glfwGetError(const char** description)
{
    _GLFWerror* error;
    int code = GLFW_NO_ERROR;

    if (description)
        *description = NULL;

    if (_glfw.initialized)
        error = _glfwPlatformGetTls(&_glfw.errorSlot);
    else
        error = &_glfwMainThreadError;

    if (error)
    {
        code = error->code;
        error->code = GLFW_NO_ERROR;
        if (description && code)
            *description = error->description;
    }

    return code;
}

GLFWAPI GLFWerrorfun glfwSetErrorCallback(GLFWerrorfun cbfun)
{
    _GLFW_SWAP_POINTERS(_glfwErrorCallback, cbfun);
    return cbfun;
}

