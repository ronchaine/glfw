//========================================================================
// GLFW 3.4 macOS - www.glfw.org
//------------------------------------------------------------------------
// Copyright (c) 2009-2019 Camilla Löwy <elmindreda@glfw.org>
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

#include <stdint.h>
#include <dlfcn.h>

#include <Carbon/Carbon.h>
#include <CoreVideo/CVBase.h>
#include <CoreVideo/CVDisplayLink.h>

// NOTE: All of NSGL was deprecated in the 10.14 SDK
//       This disables the pointless warnings for every symbol we use
#define GL_SILENCE_DEPRECATION

#if defined(__OBJC__)
#import <Cocoa/Cocoa.h>
#else
typedef void* id;
#endif

// NOTE: Many Cocoa enum values have been renamed and we need to build across
//       SDK versions where one is unavailable or the other deprecated
//       We use the newer names in code and these macros to handle compatibility
#if MAC_OS_X_VERSION_MAX_ALLOWED < 101200
 #define NSBitmapFormatAlphaNonpremultiplied NSAlphaNonpremultipliedBitmapFormat
 #define NSEventMaskAny NSAnyEventMask
 #define NSEventMaskKeyUp NSKeyUpMask
 #define NSEventModifierFlagCapsLock NSAlphaShiftKeyMask
 #define NSEventModifierFlagCommand NSCommandKeyMask
 #define NSEventModifierFlagControl NSControlKeyMask
 #define NSEventModifierFlagDeviceIndependentFlagsMask NSDeviceIndependentModifierFlagsMask
 #define NSEventModifierFlagOption NSAlternateKeyMask
 #define NSEventModifierFlagShift NSShiftKeyMask
 #define NSEventTypeApplicationDefined NSApplicationDefined
 #define NSWindowStyleMaskBorderless NSBorderlessWindowMask
 #define NSWindowStyleMaskClosable NSClosableWindowMask
 #define NSWindowStyleMaskMiniaturizable NSMiniaturizableWindowMask
 #define NSWindowStyleMaskResizable NSResizableWindowMask
 #define NSWindowStyleMaskTitled NSTitledWindowMask
#endif

typedef VkFlags VkMacOSSurfaceCreateFlagsMVK;

typedef struct VkMacOSSurfaceCreateInfoMVK
{
    VkStructureType                 sType;
    const void*                     pNext;
    VkMacOSSurfaceCreateFlagsMVK    flags;
    const void*                     pView;
} VkMacOSSurfaceCreateInfoMVK;

typedef VkResult (APIENTRY *PFN_vkCreateMacOSSurfaceMVK)(VkInstance,const VkMacOSSurfaceCreateInfoMVK*,const VkAllocationCallbacks*,VkSurfaceKHR*);

#include "platform_common.h"

#include "posix_thread.h"
#include "cocoa_joystick.h"
#include "nsgl_context.h"
#include "egl_context.h"
#include "osmesa_context.h"

#define _glfw_dlopen(name) dlopen(name, RTLD_LAZY | RTLD_LOCAL)
#define _glfw_dlclose(handle) dlclose(handle)
#define _glfw_dlsym(handle, name) dlsym(handle, name)

#define _GLFW_COCOA_EGL_NATIVE_WINDOW  ((EGLNativeWindowType) window->ns.view)
#define _GLFW_COCOA_EGL_NATIVE_DISPLAY EGL_DEFAULT_DISPLAY

#define _GLFW_COCOA_PLATFORM_WINDOW_STATE         _GLFWwindowNS  ns
#define _GLFW_COCOA_PLATFORM_LIBRARY_WINDOW_STATE _GLFWlibraryNS ns
#define _GLFW_COCOA_PLATFORM_LIBRARY_TIMER_STATE  _GLFWtimerNS   ns
#define _GLFW_COCOA_PLATFORM_MONITOR_STATE        _GLFWmonitorNS ns
#define _GLFW_COCOA_PLATFORM_CURSOR_STATE         _GLFWcursorNS  ns

// HIToolbox.framework pointer typedefs
#define kTISPropertyUnicodeKeyLayoutData _glfw.ns.tis.kPropertyUnicodeKeyLayoutData
typedef TISInputSourceRef (*PFN_TISCopyCurrentKeyboardLayoutInputSource)(void);
#define TISCopyCurrentKeyboardLayoutInputSource _glfw.ns.tis.CopyCurrentKeyboardLayoutInputSource
typedef void* (*PFN_TISGetInputSourceProperty)(TISInputSourceRef,CFStringRef);
#define TISGetInputSourceProperty _glfw.ns.tis.GetInputSourceProperty
typedef UInt8 (*PFN_LMGetKbdType)(void);
#define LMGetKbdType _glfw.ns.tis.GetKbdType


// Cocoa-specific per-window data
//
typedef struct _GLFWwindowNS
{
    id              object;
    id              delegate;
    id              view;
    id              layer;

    GLFWbool        maximized;
    GLFWbool        retina;

    // Cached window properties to filter out duplicate events
    int             width, height;
    int             fbWidth, fbHeight;
    float           xscale, yscale;

    // The total sum of the distances the cursor has been warped
    // since the last cursor motion event was processed
    // This is kept to counteract Cocoa doing the same internally
    double          cursorWarpDeltaX, cursorWarpDeltaY;

} _GLFWwindowNS;

// Cocoa-specific global data
//
typedef struct _GLFWlibraryNS
{
    CGEventSourceRef    eventSource;
    id                  delegate;
    GLFWbool            finishedLaunching;
    GLFWbool            cursorHidden;
    TISInputSourceRef   inputSource;
    IOHIDManagerRef     hidManager;
    id                  unicodeData;
    id                  helper;
    id                  keyUpMonitor;
    id                  nibObjects;

    char                keynames[GLFW_KEY_LAST + 1][17];
    short int           keycodes[256];
    short int           scancodes[GLFW_KEY_LAST + 1];
    char*               clipboardString;
    CGPoint             cascadePoint;
    // Where to place the cursor when re-enabled
    double              restoreCursorPosX, restoreCursorPosY;
    // The window whose disabled cursor mode is active
    _GLFWwindow*        disabledCursorWindow;

    struct {
        CFBundleRef     bundle;
        PFN_TISCopyCurrentKeyboardLayoutInputSource CopyCurrentKeyboardLayoutInputSource;
        PFN_TISGetInputSourceProperty GetInputSourceProperty;
        PFN_LMGetKbdType GetKbdType;
        CFStringRef     kPropertyUnicodeKeyLayoutData;
    } tis;

} _GLFWlibraryNS;

// Cocoa-specific per-monitor data
//
typedef struct _GLFWmonitorNS
{
    CGDirectDisplayID   displayID;
    CGDisplayModeRef    previousMode;
    uint32_t            unitNumber;
    id                  screen;

} _GLFWmonitorNS;

// Cocoa-specific per-cursor data
//
typedef struct _GLFWcursorNS
{
    id              object;

} _GLFWcursorNS;

// Cocoa-specific global timer data
//
typedef struct _GLFWtimerNS
{
    uint64_t        frequency;

} _GLFWtimerNS;


void _glfwInitTimerNS(void);

void _glfwPollMonitorsNS(void);
void _glfwSetVideoModeNS(_GLFWmonitor* monitor, const GLFWvidmode* desired);
void _glfwRestoreVideoModeNS(_GLFWmonitor* monitor);

float _glfwTransformYNS(float y);

//////////////////////////////////////////////////////////////////////////
//////                       GLFW platform API                      //////
//////////////////////////////////////////////////////////////////////////

// init
int  _glfwPlatformInitCocoa(void);
void _glfwPlatformTerminateCocoa(void);
const char* _glfwPlatformGetVersionStringCocoa(void);

// monitor
void _glfwPlatformFreeMonitorCocoa(_GLFWmonitor* monitor);
void _glfwPlatformGetMonitorPosCocoa(_GLFWmonitor* monitor, int* xpos, int* ypos);
void _glfwPlatformGetMonitorContentScaleCocoa(_GLFWmonitor* monitor,
                                              float* xscale, float* yscale);
void _glfwPlatformGetMonitorWorkareaCocoa(_GLFWmonitor* monitor, int* xpos, int* ypos, int *width, int *height);
GLFWvidmode* _glfwPlatformGetVideoModesCocoa(_GLFWmonitor* monitor, int* count);
void _glfwPlatformGetVideoModeCocoa(_GLFWmonitor* monitor, GLFWvidmode* mode);
GLFWbool _glfwPlatformGetGammaRampCocoa(_GLFWmonitor* monitor, GLFWgammaramp* ramp);
void _glfwPlatformSetGammaRampCocoa(_GLFWmonitor* monitor, const GLFWgammaramp* ramp);

// window
int _glfwPlatformCreateWindowCocoa(_GLFWwindow* window,
                                   const _GLFWwndconfig* wndconfig,
                                   const _GLFWctxconfig* ctxconfig,
                                   const _GLFWfbconfig* fbconfig);
void _glfwPlatformDestroyWindowCocoa(_GLFWwindow* window);
void _glfwPlatformSetWindowTitleCocoa(_GLFWwindow* window, const char* title);
void _glfwPlatformSetWindowIconCocoa(_GLFWwindow* window, int count, const GLFWimage* images);
void _glfwPlatformGetWindowPosCocoa(_GLFWwindow* window, int* xpos, int* ypos);
void _glfwPlatformSetWindowPosCocoa(_GLFWwindow* window, int xpos, int ypos);
void _glfwPlatformGetWindowSizeCocoa(_GLFWwindow* window, int* width, int* height);
void _glfwPlatformSetWindowSizeCocoa(_GLFWwindow* window, int width, int height);
void _glfwPlatformSetWindowSizeLimitsCocoa(_GLFWwindow* window,
                                             int minwidth, int minheight,
                                             int maxwidth, int maxheight);
void _glfwPlatformSetWindowAspectRatioCocoa(_GLFWwindow* window, int numer, int denom);
void _glfwPlatformGetFramebufferSizeCocoa(_GLFWwindow* window, int* width, int* height);
void _glfwPlatformGetWindowFrameSizeCocoa(_GLFWwindow* window,
                                            int* left, int* top,
                                            int* right, int* bottom);
void _glfwPlatformGetWindowContentScaleCocoa(_GLFWwindow* window, float* xscale, float* yscale);
void _glfwPlatformIconifyWindowCocoa(_GLFWwindow* window);
void _glfwPlatformRestoreWindowCocoa(_GLFWwindow* window);
void _glfwPlatformMaximizeWindowCocoa(_GLFWwindow* window);
void _glfwPlatformShowWindowCocoa(_GLFWwindow* window);
void _glfwPlatformHideWindowCocoa(_GLFWwindow* window);
void _glfwPlatformRequestWindowAttentionCocoa(_GLFWwindow* window);
void _glfwPlatformFocusWindowCocoa(_GLFWwindow* window);
void _glfwPlatformSetWindowMonitorCocoa(_GLFWwindow* window,
                                        _GLFWmonitor* monitor,
                                        int xpos, int ypos,
                                        int width, int height,
                                        int refreshRate);
int _glfwPlatformCreateCursorCocoa(_GLFWcursor* cursor,
                                   const GLFWimage* image,
                                   int xhot, int yhot);
int _glfwPlatformWindowFocusedCocoa(_GLFWwindow* window);
int _glfwPlatformWindowIconifiedCocoa(_GLFWwindow* window);
int _glfwPlatformWindowVisibleCocoa(_GLFWwindow* window);
int _glfwPlatformWindowMaximizedCocoa(_GLFWwindow* window);
int _glfwPlatformWindowHoveredCocoa(_GLFWwindow* window);
int _glfwPlatformFramebufferTransparentCocoa(_GLFWwindow* window);
void _glfwPlatformSetWindowResizableCocoa(_GLFWwindow* window, GLFWbool enabled);
void _glfwPlatformSetWindowDecoratedCocoa(_GLFWwindow* window, GLFWbool enabled);
void _glfwPlatformSetWindowFloatingCocoa(_GLFWwindow* window, GLFWbool enabled);
float _glfwPlatformGetWindowOpacityCocoa(_GLFWwindow* window);
void _glfwPlatformSetWindowOpacityCocoa(_GLFWwindow* window, float opacity);
void _glfwPlatformSetRawMouseMotionCocoa(_GLFWwindow *window, GLFWbool enabled);
GLFWbool _glfwPlatformRawMouseMotionSupportedCocoa(void);
void _glfwPlatformPollEventsCocoa(void);
void _glfwPlatformWaitEventsCocoa(void);
void _glfwPlatformWaitEventsTimeoutCocoa(double timeout);
void _glfwPlatformPostEmptyEventCocoa(void);
void _glfwPlatformGetCursorPosCocoa(_GLFWwindow* window, double* xpos, double* ypos);
void _glfwPlatformSetCursorPosCocoa(_GLFWwindow* window, double x, double y);
void _glfwPlatformSetCursorModeCocoa(_GLFWwindow* window, int mode);
const char* _glfwPlatformGetScancodeNameCocoa(int scancode);
int _glfwPlatformGetKeyScancodeCocoa(int key);
int _glfwPlatformCreateStandardCursorCocoa(_GLFWcursor* cursor, int shape);
void _glfwPlatformDestroyCursorCocoa(_GLFWcursor* cursor);
void _glfwPlatformSetCursorCocoa(_GLFWwindow* window, _GLFWcursor* cursor);
void _glfwPlatformSetClipboardStringCocoa(const char* string);
const char* _glfwPlatformGetClipboardStringCocoa(void);
void _glfwPlatformGetRequiredInstanceExtensionsCocoa(char** extensions);
int _glfwPlatformGetPhysicalDevicePresentationSupportCocoa(VkInstance instance,
                                                           VkPhysicalDevice device,
                                                           uint32_t queuefamily);
VkResult _glfwPlatformCreateWindowSurfaceCocoa(VkInstance instance,
                                               _GLFWwindow* window,
                                               const VkAllocationCallbacks* allocator,
                                               VkSurfaceKHR* surface);

static const struct _GLFWplatformfunctions _glfwFunctionsCocoa = {
    ._glfwPlatformInitFunc = _glfwPlatformInitCocoa,
    ._glfwPlatformTerminateFunc = _glfwPlatformTerminateCocoa,
    ._glfwPlatformGetVersionStringFunc = _glfwPlatformGetVersionStringCocoa,

    ._glfwPlatformFreeMonitorFunc = _glfwPlatformFreeMonitorCocoa,
    ._glfwPlatformGetMonitorPosFunc = _glfwPlatformGetMonitorPosCocoa,
    ._glfwPlatformGetMonitorContentScaleFunc = _glfwPlatformGetMonitorContentScaleCocoa,
    ._glfwPlatformGetMonitorWorkareaFunc = _glfwPlatformGetMonitorWorkareaCocoa,
    ._glfwPlatformGetVideoModesFunc = _glfwPlatformGetVideoModesCocoa,
    ._glfwPlatformGetVideoModeFunc = _glfwPlatformGetVideoModeCocoa,
    ._glfwPlatformGetGammaRampFunc = _glfwPlatformGetGammaRampCocoa,
    ._glfwPlatformSetGammaRampFunc = _glfwPlatformSetGammaRampCocoa,

    ._glfwPlatformCreateWindowFunc = _glfwPlatformCreateWindowCocoa,
    ._glfwPlatformDestroyWindowFunc = _glfwPlatformDestroyWindowCocoa,
    ._glfwPlatformSetWindowTitleFunc = _glfwPlatformSetWindowTitleCocoa,
    ._glfwPlatformSetWindowIconFunc = _glfwPlatformSetWindowIconCocoa,
    ._glfwPlatformGetWindowPosFunc = _glfwPlatformGetWindowPosCocoa,
    ._glfwPlatformSetWindowPosFunc = _glfwPlatformSetWindowPosCocoa,
    ._glfwPlatformGetWindowSizeFunc = _glfwPlatformGetWindowSizeCocoa,
    ._glfwPlatformSetWindowSizeFunc = _glfwPlatformSetWindowSizeCocoa,
    ._glfwPlatformSetWindowSizeLimitsFunc = _glfwPlatformSetWindowSizeLimitsCocoa,
    ._glfwPlatformSetWindowAspectRatioFunc = _glfwPlatformSetWindowAspectRatioCocoa,
    ._glfwPlatformGetFramebufferSizeFunc = _glfwPlatformGetFramebufferSizeCocoa,
    ._glfwPlatformGetWindowFrameSizeFunc = _glfwPlatformGetWindowFrameSizeCocoa,
    ._glfwPlatformGetWindowContentScaleFunc = _glfwPlatformGetWindowContentScaleCocoa,
    ._glfwPlatformIconifyWindowFunc = _glfwPlatformIconifyWindowCocoa,
    ._glfwPlatformRestoreWindowFunc = _glfwPlatformRestoreWindowCocoa,
    ._glfwPlatformMaximizeWindowFunc = _glfwPlatformMaximizeWindowCocoa,
    ._glfwPlatformShowWindowFunc = _glfwPlatformShowWindowCocoa,
    ._glfwPlatformHideWindowFunc = _glfwPlatformHideWindowCocoa,
    ._glfwPlatformRequestWindowAttentionFunc = _glfwPlatformRequestWindowAttentionCocoa,
    ._glfwPlatformFocusWindowFunc = _glfwPlatformFocusWindowCocoa,
    ._glfwPlatformSetWindowMonitorFunc = _glfwPlatformSetWindowMonitorCocoa,
    ._glfwPlatformCreateCursorFunc = _glfwPlatformCreateCursorCocoa,
    ._glfwPlatformWindowFocusedFunc = _glfwPlatformWindowFocusedCocoa,
    ._glfwPlatformWindowIconifiedFunc = _glfwPlatformWindowIconifiedCocoa,
    ._glfwPlatformWindowVisibleFunc =  _glfwPlatformWindowVisibleCocoa,
    ._glfwPlatformWindowMaximizedFunc = _glfwPlatformWindowMaximizedCocoa,
    ._glfwPlatformWindowHoveredFunc = _glfwPlatformWindowHoveredCocoa,
    ._glfwPlatformFramebufferTransparentFunc = _glfwPlatformFramebufferTransparentCocoa,
    ._glfwPlatformSetWindowResizableFunc = _glfwPlatformSetWindowResizableCocoa,
    ._glfwPlatformSetWindowDecoratedFunc = _glfwPlatformSetWindowDecoratedCocoa,
    ._glfwPlatformSetWindowFloatingFunc = _glfwPlatformSetWindowFloatingCocoa,
    ._glfwPlatformGetWindowOpacityFunc = _glfwPlatformGetWindowOpacityCocoa,
    ._glfwPlatformSetWindowOpacityFunc = _glfwPlatformSetWindowOpacityCocoa,
    ._glfwPlatformSetRawMouseMotionFunc = _glfwPlatformSetRawMouseMotionCocoa,
    ._glfwPlatformRawMouseMotionSupportedFunc = _glfwPlatformRawMouseMotionSupportedCocoa,
    ._glfwPlatformPollEventsFunc = _glfwPlatformPollEventsCocoa,
    ._glfwPlatformWaitEventsFunc = _glfwPlatformWaitEventsCocoa,
    ._glfwPlatformWaitEventsTimeoutFunc = _glfwPlatformWaitEventsTimeoutCocoa,
    ._glfwPlatformPostEmptyEventFunc = _glfwPlatformPostEmptyEventCocoa,
    ._glfwPlatformGetCursorPosFunc = _glfwPlatformGetCursorPosCocoa,
    ._glfwPlatformSetCursorPosFunc = _glfwPlatformSetCursorPosCocoa,
    ._glfwPlatformSetCursorModeFunc = _glfwPlatformSetCursorModeCocoa,
    ._glfwPlatformGetScancodeNameFunc = _glfwPlatformGetScancodeNameCocoa,
    ._glfwPlatformGetKeyScancodeFunc = _glfwPlatformGetKeyScancodeCocoa,
    ._glfwPlatformCreateStandardCursorFunc = _glfwPlatformCreateStandardCursorCocoa,
    ._glfwPlatformDestroyCursorFunc = _glfwPlatformDestroyCursorCocoa,
    ._glfwPlatformSetCursorFunc = _glfwPlatformSetCursorCocoa,
    ._glfwPlatformSetClipboardStringFunc = _glfwPlatformSetClipboardStringCocoa,
    ._glfwPlatformGetClipboardStringFunc = _glfwPlatformGetClipboardStringCocoa,
    ._glfwPlatformGetRequiredInstanceExtensionsFunc = _glfwPlatformGetRequiredInstanceExtensionsCocoa,
    ._glfwPlatformGetPhysicalDevicePresentationSupportFunc = _glfwPlatformGetPhysicalDevicePresentationSupportCocoa,
    ._glfwPlatformCreateWindowSurfaceFunc = _glfwPlatformCreateWindowSurfaceCocoa,

    ._glfwPlatformCreateTlsFunc = _glfwPlatformCreateTlsCocoa,
    ._glfwPlatformDestroyTlsFunc = _glfwPlatformDestroyTlsCocoa,
    ._glfwPlatformGetTlsFunc = _glfwPlatformGetTlsCocoa,
    ._glfwPlatformSetTlsFunc = _glfwPlatformSetTlsCocoa,

    ._glfwPlatformCreateMutexFunc = _glfwPlatformCreateMutexCocoa,
    ._glfwPlatformDestroyMutexFunc = _glfwPlatformDestroyMutexCocoa,
    ._glfwPlatformLockMutexFunc = _glfwPlatformLockMutexCocoa,
    ._glfwPlatformUnlockMutexFunc = _glfwPlatformUnlockMutexCocoa,

    ._glfwPlatformGetTimerValueFunc = _glfwPlatformGetTimerValueCocoa,
    ._glfwPlatformGetTimerFrequencyFunc = _glfwPlatformGetTimerFrequencyCocoa,
    
    ._glfwPlatformPollJoystickFunc = _glfwPlatformPollJoystickCocoa,
    ._glfwPlatformUpdateGamepadGUIDFunc = _glfwPlatformUpdateGamepadGUIDCocoa,
};
