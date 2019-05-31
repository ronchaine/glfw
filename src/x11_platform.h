//========================================================================
// GLFW 3.4 X11 - www.glfw.org
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2019 Camilla Löwy <elmindreda@glfw.org>
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

#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <dlfcn.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/Xcursor/Xcursor.h>

// The XRandR extension provides mode setting and gamma control
#include <X11/extensions/Xrandr.h>

// The Xkb extension provides improved keyboard support
#include <X11/XKBlib.h>

// The Xinerama extension provides legacy monitor indices
#include <X11/extensions/Xinerama.h>

// The XInput extension provides raw mouse motion input
#include <X11/extensions/XInput2.h>

#include "platform_common.h"

typedef XRRCrtcGamma* (* PFN_XRRAllocGamma)(int);
typedef void (* PFN_XRRFreeCrtcInfo)(XRRCrtcInfo*);
typedef void (* PFN_XRRFreeGamma)(XRRCrtcGamma*);
typedef void (* PFN_XRRFreeOutputInfo)(XRROutputInfo*);
typedef void (* PFN_XRRFreeScreenResources)(XRRScreenResources*);
typedef XRRCrtcGamma* (* PFN_XRRGetCrtcGamma)(Display*,RRCrtc);
typedef int (* PFN_XRRGetCrtcGammaSize)(Display*,RRCrtc);
typedef XRRCrtcInfo* (* PFN_XRRGetCrtcInfo) (Display*,XRRScreenResources*,RRCrtc);
typedef XRROutputInfo* (* PFN_XRRGetOutputInfo)(Display*,XRRScreenResources*,RROutput);
typedef RROutput (* PFN_XRRGetOutputPrimary)(Display*,Window);
typedef XRRScreenResources* (* PFN_XRRGetScreenResourcesCurrent)(Display*,Window);
typedef Bool (* PFN_XRRQueryExtension)(Display*,int*,int*);
typedef Status (* PFN_XRRQueryVersion)(Display*,int*,int*);
typedef void (* PFN_XRRSelectInput)(Display*,Window,int);
typedef Status (* PFN_XRRSetCrtcConfig)(Display*,XRRScreenResources*,RRCrtc,Time,int,int,RRMode,Rotation,RROutput*,int);
typedef void (* PFN_XRRSetCrtcGamma)(Display*,RRCrtc,XRRCrtcGamma*);
typedef int (* PFN_XRRUpdateConfiguration)(XEvent*);
#define XRRAllocGamma _glfw.x11.randr.AllocGamma
#define XRRFreeCrtcInfo _glfw.x11.randr.FreeCrtcInfo
#define XRRFreeGamma _glfw.x11.randr.FreeGamma
#define XRRFreeOutputInfo _glfw.x11.randr.FreeOutputInfo
#define XRRFreeScreenResources _glfw.x11.randr.FreeScreenResources
#define XRRGetCrtcGamma _glfw.x11.randr.GetCrtcGamma
#define XRRGetCrtcGammaSize _glfw.x11.randr.GetCrtcGammaSize
#define XRRGetCrtcInfo _glfw.x11.randr.GetCrtcInfo
#define XRRGetOutputInfo _glfw.x11.randr.GetOutputInfo
#define XRRGetOutputPrimary _glfw.x11.randr.GetOutputPrimary
#define XRRGetScreenResourcesCurrent _glfw.x11.randr.GetScreenResourcesCurrent
#define XRRQueryExtension _glfw.x11.randr.QueryExtension
#define XRRQueryVersion _glfw.x11.randr.QueryVersion
#define XRRSelectInput _glfw.x11.randr.SelectInput
#define XRRSetCrtcConfig _glfw.x11.randr.SetCrtcConfig
#define XRRSetCrtcGamma _glfw.x11.randr.SetCrtcGamma
#define XRRUpdateConfiguration _glfw.x11.randr.UpdateConfiguration

typedef XcursorImage* (* PFN_XcursorImageCreate)(int,int);
typedef void (* PFN_XcursorImageDestroy)(XcursorImage*);
typedef Cursor (* PFN_XcursorImageLoadCursor)(Display*,const XcursorImage*);
#define XcursorImageCreate _glfw.x11.xcursor.ImageCreate
#define XcursorImageDestroy _glfw.x11.xcursor.ImageDestroy
#define XcursorImageLoadCursor _glfw.x11.xcursor.ImageLoadCursor

typedef Bool (* PFN_XineramaIsActive)(Display*);
typedef Bool (* PFN_XineramaQueryExtension)(Display*,int*,int*);
typedef XineramaScreenInfo* (* PFN_XineramaQueryScreens)(Display*,int*);
#define XineramaIsActive _glfw.x11.xinerama.IsActive
#define XineramaQueryExtension _glfw.x11.xinerama.QueryExtension
#define XineramaQueryScreens _glfw.x11.xinerama.QueryScreens

typedef XID xcb_window_t;
typedef XID xcb_visualid_t;
typedef struct xcb_connection_t xcb_connection_t;
typedef xcb_connection_t* (* PFN_XGetXCBConnection)(Display*);
#define XGetXCBConnection _glfw.x11.x11xcb.GetXCBConnection

typedef Bool (* PFN_XF86VidModeQueryExtension)(Display*,int*,int*);
typedef Bool (* PFN_XF86VidModeGetGammaRamp)(Display*,int,int,unsigned short*,unsigned short*,unsigned short*);
typedef Bool (* PFN_XF86VidModeSetGammaRamp)(Display*,int,int,unsigned short*,unsigned short*,unsigned short*);
typedef Bool (* PFN_XF86VidModeGetGammaRampSize)(Display*,int,int*);
#define XF86VidModeQueryExtension _glfw.x11.vidmode.QueryExtension
#define XF86VidModeGetGammaRamp _glfw.x11.vidmode.GetGammaRamp
#define XF86VidModeSetGammaRamp _glfw.x11.vidmode.SetGammaRamp
#define XF86VidModeGetGammaRampSize _glfw.x11.vidmode.GetGammaRampSize

typedef Status (* PFN_XIQueryVersion)(Display*,int*,int*);
typedef int (* PFN_XISelectEvents)(Display*,Window,XIEventMask*,int);
#define XIQueryVersion _glfw.x11.xi.QueryVersion
#define XISelectEvents _glfw.x11.xi.SelectEvents

typedef Bool (* PFN_XRenderQueryExtension)(Display*,int*,int*);
typedef Status (* PFN_XRenderQueryVersion)(Display*dpy,int*,int*);
typedef XRenderPictFormat* (* PFN_XRenderFindVisualFormat)(Display*,Visual const*);
#define XRenderQueryExtension _glfw.x11.xrender.QueryExtension
#define XRenderQueryVersion _glfw.x11.xrender.QueryVersion
#define XRenderFindVisualFormat _glfw.x11.xrender.FindVisualFormat

typedef VkFlags VkXlibSurfaceCreateFlagsKHR;
typedef VkFlags VkXcbSurfaceCreateFlagsKHR;

typedef struct VkXlibSurfaceCreateInfoKHR
{
    VkStructureType             sType;
    const void*                 pNext;
    VkXlibSurfaceCreateFlagsKHR flags;
    Display*                    dpy;
    Window                      window;
} VkXlibSurfaceCreateInfoKHR;

typedef struct VkXcbSurfaceCreateInfoKHR
{
    VkStructureType             sType;
    const void*                 pNext;
    VkXcbSurfaceCreateFlagsKHR  flags;
    xcb_connection_t*           connection;
    xcb_window_t                window;
} VkXcbSurfaceCreateInfoKHR;

typedef VkResult (APIENTRY *PFN_vkCreateXlibSurfaceKHR)(VkInstance,const VkXlibSurfaceCreateInfoKHR*,const VkAllocationCallbacks*,VkSurfaceKHR*);
typedef VkBool32 (APIENTRY *PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR)(VkPhysicalDevice,uint32_t,Display*,VisualID);
typedef VkResult (APIENTRY *PFN_vkCreateXcbSurfaceKHR)(VkInstance,const VkXcbSurfaceCreateInfoKHR*,const VkAllocationCallbacks*,VkSurfaceKHR*);
typedef VkBool32 (APIENTRY *PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR)(VkPhysicalDevice,uint32_t,xcb_connection_t*,xcb_visualid_t);

#include "posix_thread.h"
#include "posix_time.h"
#include "xkb_unicode.h"
#include "glx_context.h"
#include "egl_context.h"
#include "osmesa_context.h"
#if defined(__linux__)
#include "linux_joystick.h"
#else
#include "null_joystick.h"
#endif

#define _glfw_dlopen(name) dlopen(name, RTLD_LAZY | RTLD_LOCAL)
#define _glfw_dlclose(handle) dlclose(handle)
#define _glfw_dlsym(handle, name) dlsym(handle, name)

#define _GLFW_X11_EGL_NATIVE_WINDOW  ((EGLNativeWindowType) window->x11.handle)
#define _GLFW_X11_EGL_NATIVE_DISPLAY ((EGLNativeDisplayType) _glfw.x11.display)

#define _GLFW_X11_PLATFORM_WINDOW_STATE         _GLFWwindowX11  x11
#define _GLFW_X11_PLATFORM_LIBRARY_WINDOW_STATE _GLFWlibraryX11 x11
#define _GLFW_X11_PLATFORM_MONITOR_STATE        _GLFWmonitorX11 x11
#define _GLFW_X11_PLATFORM_CURSOR_STATE         _GLFWcursorX11  x11

//#ifndef _GLFW_DEFAULT_PLATFORM_FUNCTIONS
//#define _GLFW_DEFAULT_PLATFORM              GLFW_PLATFORM_X11
//#define _GLFW_DEFAULT_PLATFORM_FUNCTIONS    _glfwFunctionsX11
//#endif

// X11-specific per-window data
//
typedef struct _GLFWwindowX11
{
    Colormap        colormap;
    Window          handle;
    XIC             ic;

    GLFWbool        overrideRedirect;
    GLFWbool        iconified;
    GLFWbool        maximized;

    // Whether the visual supports framebuffer transparency
    GLFWbool        transparent;

    // Cached position and size used to filter out duplicate events
    int             width, height;
    int             xpos, ypos;

    // The last received cursor position, regardless of source
    int             lastCursorPosX, lastCursorPosY;
    // The last position the cursor was warped to by GLFW
    int             warpCursorPosX, warpCursorPosY;

    // The time of the last KeyPress event
    Time            lastKeyTime;

} _GLFWwindowX11;

// X11-specific global data
//
typedef struct _GLFWlibraryX11
{
    Display*        display;
    int             screen;
    Window          root;

    // System content scale
    float           contentScaleX, contentScaleY;
    // Helper window for IPC
    Window          helperWindowHandle;
    // Invisible cursor for hidden cursor mode
    Cursor          hiddenCursorHandle;
    // Context for mapping window XIDs to _GLFWwindow pointers
    XContext        context;
    // XIM input method
    XIM             im;
    // Most recent error code received by X error handler
    int             errorCode;
    // Primary selection string (while the primary selection is owned)
    char*           primarySelectionString;
    // Clipboard string (while the selection is owned)
    char*           clipboardString;
    // Key name string
    char            keyName[5];
    // X11 keycode to GLFW key LUT
    short int       keycodes[256];
    // GLFW key to X11 keycode LUT
    short int       scancodes[GLFW_KEY_LAST + 1];
    // Where to place the cursor when re-enabled
    double          restoreCursorPosX, restoreCursorPosY;
    // The window whose disabled cursor mode is active
    _GLFWwindow*    disabledCursorWindow;

    // Window manager atoms
    Atom            WM_PROTOCOLS;
    Atom            WM_STATE;
    Atom            WM_DELETE_WINDOW;
    Atom            NET_WM_NAME;
    Atom            NET_WM_ICON_NAME;
    Atom            NET_WM_ICON;
    Atom            NET_WM_PID;
    Atom            NET_WM_PING;
    Atom            NET_WM_WINDOW_TYPE;
    Atom            NET_WM_WINDOW_TYPE_NORMAL;
    Atom            NET_WM_STATE;
    Atom            NET_WM_STATE_ABOVE;
    Atom            NET_WM_STATE_FULLSCREEN;
    Atom            NET_WM_STATE_MAXIMIZED_VERT;
    Atom            NET_WM_STATE_MAXIMIZED_HORZ;
    Atom            NET_WM_STATE_DEMANDS_ATTENTION;
    Atom            NET_WM_BYPASS_COMPOSITOR;
    Atom            NET_WM_FULLSCREEN_MONITORS;
    Atom            NET_WM_WINDOW_OPACITY;
    Atom            NET_WM_CM_Sx;
    Atom            NET_WORKAREA;
    Atom            NET_CURRENT_DESKTOP;
    Atom            NET_ACTIVE_WINDOW;
    Atom            NET_FRAME_EXTENTS;
    Atom            NET_REQUEST_FRAME_EXTENTS;
    Atom            MOTIF_WM_HINTS;

    // Xdnd (drag and drop) atoms
    Atom            XdndAware;
    Atom            XdndEnter;
    Atom            XdndPosition;
    Atom            XdndStatus;
    Atom            XdndActionCopy;
    Atom            XdndDrop;
    Atom            XdndFinished;
    Atom            XdndSelection;
    Atom            XdndTypeList;
    Atom            text_uri_list;

    // Selection (clipboard) atoms
    Atom            TARGETS;
    Atom            MULTIPLE;
    Atom            INCR;
    Atom            CLIPBOARD;
    Atom            PRIMARY;
    Atom            CLIPBOARD_MANAGER;
    Atom            SAVE_TARGETS;
    Atom            NULL_;
    Atom            UTF8_STRING;
    Atom            COMPOUND_STRING;
    Atom            ATOM_PAIR;
    Atom            GLFW_SELECTION;

    struct {
        GLFWbool    available;
        void*       handle;
        int         eventBase;
        int         errorBase;
        int         major;
        int         minor;
        GLFWbool    gammaBroken;
        GLFWbool    monitorBroken;
        PFN_XRRAllocGamma AllocGamma;
        PFN_XRRFreeCrtcInfo FreeCrtcInfo;
        PFN_XRRFreeGamma FreeGamma;
        PFN_XRRFreeOutputInfo FreeOutputInfo;
        PFN_XRRFreeScreenResources FreeScreenResources;
        PFN_XRRGetCrtcGamma GetCrtcGamma;
        PFN_XRRGetCrtcGammaSize GetCrtcGammaSize;
        PFN_XRRGetCrtcInfo GetCrtcInfo;
        PFN_XRRGetOutputInfo GetOutputInfo;
        PFN_XRRGetOutputPrimary GetOutputPrimary;
        PFN_XRRGetScreenResourcesCurrent GetScreenResourcesCurrent;
        PFN_XRRQueryExtension QueryExtension;
        PFN_XRRQueryVersion QueryVersion;
        PFN_XRRSelectInput SelectInput;
        PFN_XRRSetCrtcConfig SetCrtcConfig;
        PFN_XRRSetCrtcGamma SetCrtcGamma;
        PFN_XRRUpdateConfiguration UpdateConfiguration;
    } randr;

    struct {
        GLFWbool    available;
        GLFWbool    detectable;
        int         majorOpcode;
        int         eventBase;
        int         errorBase;
        int         major;
        int         minor;
    } xkb;

    struct {
        int         count;
        int         timeout;
        int         interval;
        int         blanking;
        int         exposure;
    } saver;

    struct {
        int         version;
        Window      source;
        Atom        format;
    } xdnd;

    struct {
        void*       handle;
        PFN_XcursorImageCreate ImageCreate;
        PFN_XcursorImageDestroy ImageDestroy;
        PFN_XcursorImageLoadCursor ImageLoadCursor;
    } xcursor;

    struct {
        GLFWbool    available;
        void*       handle;
        int         major;
        int         minor;
        PFN_XineramaIsActive IsActive;
        PFN_XineramaQueryExtension QueryExtension;
        PFN_XineramaQueryScreens QueryScreens;
    } xinerama;

    struct {
        void*       handle;
        PFN_XGetXCBConnection GetXCBConnection;
    } x11xcb;

    struct {
        GLFWbool    available;
        void*       handle;
        int         eventBase;
        int         errorBase;
        PFN_XF86VidModeQueryExtension QueryExtension;
        PFN_XF86VidModeGetGammaRamp GetGammaRamp;
        PFN_XF86VidModeSetGammaRamp SetGammaRamp;
        PFN_XF86VidModeGetGammaRampSize GetGammaRampSize;
    } vidmode;

    struct {
        GLFWbool    available;
        void*       handle;
        int         majorOpcode;
        int         eventBase;
        int         errorBase;
        int         major;
        int         minor;
        PFN_XIQueryVersion QueryVersion;
        PFN_XISelectEvents SelectEvents;
    } xi;

    struct {
        GLFWbool    available;
        void*       handle;
        int         major;
        int         minor;
        int         eventBase;
        int         errorBase;
        PFN_XRenderQueryExtension QueryExtension;
        PFN_XRenderQueryVersion QueryVersion;
        PFN_XRenderFindVisualFormat FindVisualFormat;
    } xrender;

} _GLFWlibraryX11;

// X11-specific per-monitor data
//
typedef struct _GLFWmonitorX11
{
    RROutput        output;
    RRCrtc          crtc;
    RRMode          oldMode;

    // Index of corresponding Xinerama screen,
    // for EWMH full screen window placement
    int             index;

} _GLFWmonitorX11;

// X11-specific per-cursor data
//
typedef struct _GLFWcursorX11
{
    Cursor handle;

} _GLFWcursorX11;


void _glfwPollMonitorsX11(void);
void _glfwSetVideoModeX11(_GLFWmonitor* monitor, const GLFWvidmode* desired);
void _glfwRestoreVideoModeX11(_GLFWmonitor* monitor);

Cursor _glfwCreateCursorX11(const GLFWimage* image, int xhot, int yhot);

unsigned long _glfwGetWindowPropertyX11(Window window,
                                        Atom property,
                                        Atom type,
                                        unsigned char** value);
GLFWbool _glfwIsVisualTransparentX11(Visual* visual);

void _glfwGrabErrorHandlerX11(void);
void _glfwReleaseErrorHandlerX11(void);
void _glfwInputErrorX11(int error, const char* message);

void _glfwPushSelectionToManagerX11(void);


//////////////////////////////////////////////////////////////////////////
//////                       GLFW platform API                      //////
//////////////////////////////////////////////////////////////////////////

// init
int  _glfwPlatformInitX11(void);
void _glfwPlatformTerminateX11(void);
const char* _glfwPlatformGetVersionStringX11(void);

// monitor
void _glfwPlatformFreeMonitorX11(_GLFWmonitor* monitor);
void _glfwPlatformGetMonitorPosX11(_GLFWmonitor* monitor, int* xpos, int* ypos);
void _glfwPlatformGetMonitorContentScaleX11(_GLFWmonitor* monitor,
                                            float* xscale, float* yscale);
void _glfwPlatformGetMonitorWorkareaX11(_GLFWmonitor* monitor, int* xpos, int* ypos, int *width, int *height);
GLFWvidmode* _glfwPlatformGetVideoModesX11(_GLFWmonitor* monitor, int* count);
void _glfwPlatformGetVideoModeX11(_GLFWmonitor* monitor, GLFWvidmode* mode);
GLFWbool _glfwPlatformGetGammaRampX11(_GLFWmonitor* monitor, GLFWgammaramp* ramp);
void _glfwPlatformSetGammaRampX11(_GLFWmonitor* monitor, const GLFWgammaramp* ramp);

// window
int _glfwPlatformCreateWindowX11(_GLFWwindow* window,
                                     const _GLFWwndconfig* wndconfig,
                                     const _GLFWctxconfig* ctxconfig,
                                     const _GLFWfbconfig* fbconfig);
void _glfwPlatformDestroyWindowX11(_GLFWwindow* window);
void _glfwPlatformSetWindowTitleX11(_GLFWwindow* window, const char* title);
void _glfwPlatformSetWindowIconX11(_GLFWwindow* window, int count, const GLFWimage* images);
void _glfwPlatformGetWindowPosX11(_GLFWwindow* window, int* xpos, int* ypos);
void _glfwPlatformSetWindowPosX11(_GLFWwindow* window, int xpos, int ypos);
void _glfwPlatformGetWindowSizeX11(_GLFWwindow* window, int* width, int* height);
void _glfwPlatformSetWindowSizeX11(_GLFWwindow* window, int width, int height);
void _glfwPlatformSetWindowSizeLimitsX11(_GLFWwindow* window,
                                             int minwidth, int minheight,
                                             int maxwidth, int maxheight);
void _glfwPlatformSetWindowAspectRatioX11(_GLFWwindow* window, int numer, int denom);
void _glfwPlatformGetFramebufferSizeX11(_GLFWwindow* window, int* width, int* height);
void _glfwPlatformGetWindowFrameSizeX11(_GLFWwindow* window,
                                            int* left, int* top,
                                            int* right, int* bottom);
void _glfwPlatformGetWindowContentScaleX11(_GLFWwindow* window, float* xscale, float* yscale);
void _glfwPlatformIconifyWindowX11(_GLFWwindow* window);
void _glfwPlatformRestoreWindowX11(_GLFWwindow* window);
void _glfwPlatformMaximizeWindowX11(_GLFWwindow* window);
void _glfwPlatformShowWindowX11(_GLFWwindow* window);
void _glfwPlatformHideWindowX11(_GLFWwindow* window);
void _glfwPlatformRequestWindowAttentionX11(_GLFWwindow* window);
void _glfwPlatformFocusWindowX11(_GLFWwindow* window);
void _glfwPlatformSetWindowMonitorX11(_GLFWwindow* window,
                                          _GLFWmonitor* monitor,
                                          int xpos, int ypos,
                                          int width, int height,
                                          int refreshRate);
int _glfwPlatformCreateCursorX11(_GLFWcursor* cursor,
                                     const GLFWimage* image,
                                     int xhot, int yhot);
int _glfwPlatformWindowFocusedX11(_GLFWwindow* window);
int _glfwPlatformWindowIconifiedX11(_GLFWwindow* window);
int _glfwPlatformWindowVisibleX11(_GLFWwindow* window);
int _glfwPlatformWindowMaximizedX11(_GLFWwindow* window);
int _glfwPlatformWindowHoveredX11(_GLFWwindow* window);
int _glfwPlatformFramebufferTransparentX11(_GLFWwindow* window);
void _glfwPlatformSetWindowResizableX11(_GLFWwindow* window, GLFWbool enabled);
void _glfwPlatformSetWindowDecoratedX11(_GLFWwindow* window, GLFWbool enabled);
void _glfwPlatformSetWindowFloatingX11(_GLFWwindow* window, GLFWbool enabled);
float _glfwPlatformGetWindowOpacityX11(_GLFWwindow* window);
void _glfwPlatformSetWindowOpacityX11(_GLFWwindow* window, float opacity);
void _glfwPlatformSetRawMouseMotionX11(_GLFWwindow *window, GLFWbool enabled);
GLFWbool _glfwPlatformRawMouseMotionSupportedX11(void);
void _glfwPlatformPollEventsX11(void);
void _glfwPlatformWaitEventsX11(void);
void _glfwPlatformWaitEventsTimeoutX11(double timeout);
void _glfwPlatformPostEmptyEventX11(void);
void _glfwPlatformGetCursorPosX11(_GLFWwindow* window, double* xpos, double* ypos);
void _glfwPlatformSetCursorPosX11(_GLFWwindow* window, double x, double y);
void _glfwPlatformSetCursorModeX11(_GLFWwindow* window, int mode);
const char* _glfwPlatformGetScancodeNameX11(int scancode);
int _glfwPlatformGetKeyScancodeX11(int key);
int _glfwPlatformCreateStandardCursorX11(_GLFWcursor* cursor, int shape);
void _glfwPlatformDestroyCursorX11(_GLFWcursor* cursor);
void _glfwPlatformSetCursorX11(_GLFWwindow* window, _GLFWcursor* cursor);
void _glfwPlatformSetClipboardStringX11(const char* string);
const char* _glfwPlatformGetClipboardStringX11(void);
void _glfwPlatformGetRequiredInstanceExtensionsX11(char** extensions);
int _glfwPlatformGetPhysicalDevicePresentationSupportX11(VkInstance instance,
                                                             VkPhysicalDevice device,
                                                             uint32_t queuefamily);
VkResult _glfwPlatformCreateWindowSurfaceX11(VkInstance instance,
                                                 _GLFWwindow* window,
                                                 const VkAllocationCallbacks* allocator,
                                                 VkSurfaceKHR* surface);

static const struct _GLFWplatformfunctions _glfwFunctionsX11 = {
    ._glfwPlatformInitFunc = _glfwPlatformInitX11,
    ._glfwPlatformTerminateFunc = _glfwPlatformTerminateX11,
    ._glfwPlatformGetVersionStringFunc = _glfwPlatformGetVersionStringX11,

    ._glfwPlatformFreeMonitorFunc = _glfwPlatformFreeMonitorX11,
    ._glfwPlatformGetMonitorPosFunc = _glfwPlatformGetMonitorPosX11,
    ._glfwPlatformGetMonitorContentScaleFunc = _glfwPlatformGetMonitorContentScaleX11,
    ._glfwPlatformGetMonitorWorkareaFunc = _glfwPlatformGetMonitorWorkareaX11,
    ._glfwPlatformGetVideoModesFunc = _glfwPlatformGetVideoModesX11,
    ._glfwPlatformGetVideoModeFunc = _glfwPlatformGetVideoModeX11,
    ._glfwPlatformGetGammaRampFunc = _glfwPlatformGetGammaRampX11,
    ._glfwPlatformSetGammaRampFunc = _glfwPlatformSetGammaRampX11,

    ._glfwPlatformCreateWindowFunc = _glfwPlatformCreateWindowX11,
    ._glfwPlatformDestroyWindowFunc = _glfwPlatformDestroyWindowX11,
    ._glfwPlatformSetWindowTitleFunc = _glfwPlatformSetWindowTitleX11,
    ._glfwPlatformSetWindowIconFunc = _glfwPlatformSetWindowIconX11,
    ._glfwPlatformGetWindowPosFunc = _glfwPlatformGetWindowPosX11,
    ._glfwPlatformSetWindowPosFunc = _glfwPlatformSetWindowPosX11,
    ._glfwPlatformGetWindowSizeFunc = _glfwPlatformGetWindowSizeX11,
    ._glfwPlatformSetWindowSizeFunc = _glfwPlatformSetWindowSizeX11,
    ._glfwPlatformSetWindowSizeLimitsFunc = _glfwPlatformSetWindowSizeLimitsX11,
    ._glfwPlatformSetWindowAspectRatioFunc = _glfwPlatformSetWindowAspectRatioX11,
    ._glfwPlatformGetFramebufferSizeFunc = _glfwPlatformGetFramebufferSizeX11,
    ._glfwPlatformGetWindowFrameSizeFunc = _glfwPlatformGetWindowFrameSizeX11,
    ._glfwPlatformGetWindowContentScaleFunc = _glfwPlatformGetWindowContentScaleX11,
    ._glfwPlatformIconifyWindowFunc = _glfwPlatformIconifyWindowX11,
    ._glfwPlatformRestoreWindowFunc = _glfwPlatformRestoreWindowX11,
    ._glfwPlatformMaximizeWindowFunc = _glfwPlatformMaximizeWindowX11,
    ._glfwPlatformShowWindowFunc = _glfwPlatformShowWindowX11,
    ._glfwPlatformHideWindowFunc = _glfwPlatformHideWindowX11,
    ._glfwPlatformRequestWindowAttentionFunc = _glfwPlatformRequestWindowAttentionX11,
    ._glfwPlatformFocusWindowFunc = _glfwPlatformFocusWindowX11,
    ._glfwPlatformSetWindowMonitorFunc = _glfwPlatformSetWindowMonitorX11,
    ._glfwPlatformCreateCursorFunc = _glfwPlatformCreateCursorX11,
    ._glfwPlatformWindowFocusedFunc = _glfwPlatformWindowFocusedX11,
    ._glfwPlatformWindowIconifiedFunc = _glfwPlatformWindowIconifiedX11,
    ._glfwPlatformWindowVisibleFunc =  _glfwPlatformWindowVisibleX11,
    ._glfwPlatformWindowMaximizedFunc = _glfwPlatformWindowMaximizedX11,
    ._glfwPlatformWindowHoveredFunc = _glfwPlatformWindowHoveredX11,
    ._glfwPlatformFramebufferTransparentFunc = _glfwPlatformFramebufferTransparentX11,
    ._glfwPlatformSetWindowResizableFunc = _glfwPlatformSetWindowResizableX11,
    ._glfwPlatformSetWindowDecoratedFunc = _glfwPlatformSetWindowDecoratedX11,
    ._glfwPlatformSetWindowFloatingFunc = _glfwPlatformSetWindowFloatingX11,
    ._glfwPlatformGetWindowOpacityFunc = _glfwPlatformGetWindowOpacityX11,
    ._glfwPlatformSetWindowOpacityFunc = _glfwPlatformSetWindowOpacityX11,
    ._glfwPlatformSetRawMouseMotionFunc = _glfwPlatformSetRawMouseMotionX11,
    ._glfwPlatformRawMouseMotionSupportedFunc = _glfwPlatformRawMouseMotionSupportedX11,
    ._glfwPlatformPollEventsFunc = _glfwPlatformPollEventsX11,
    ._glfwPlatformWaitEventsFunc = _glfwPlatformWaitEventsX11,
    ._glfwPlatformWaitEventsTimeoutFunc = _glfwPlatformWaitEventsTimeoutX11,
    ._glfwPlatformPostEmptyEventFunc = _glfwPlatformPostEmptyEventX11,
    ._glfwPlatformGetCursorPosFunc = _glfwPlatformGetCursorPosX11,
    ._glfwPlatformSetCursorPosFunc = _glfwPlatformSetCursorPosX11,
    ._glfwPlatformSetCursorModeFunc = _glfwPlatformSetCursorModeX11,
    ._glfwPlatformGetScancodeNameFunc = _glfwPlatformGetScancodeNameX11,
    ._glfwPlatformGetKeyScancodeFunc = _glfwPlatformGetKeyScancodeX11,
    ._glfwPlatformCreateStandardCursorFunc = _glfwPlatformCreateStandardCursorX11,
    ._glfwPlatformDestroyCursorFunc = _glfwPlatformDestroyCursorX11,
    ._glfwPlatformSetCursorFunc = _glfwPlatformSetCursorX11,
    ._glfwPlatformSetClipboardStringFunc = _glfwPlatformSetClipboardStringX11,
    ._glfwPlatformGetClipboardStringFunc = _glfwPlatformGetClipboardStringX11,
    ._glfwPlatformGetRequiredInstanceExtensionsFunc = _glfwPlatformGetRequiredInstanceExtensionsX11,
    ._glfwPlatformGetPhysicalDevicePresentationSupportFunc = _glfwPlatformGetPhysicalDevicePresentationSupportX11,
    ._glfwPlatformCreateWindowSurfaceFunc = _glfwPlatformCreateWindowSurfaceX11,

    ._glfwPlatformCreateTlsFunc = _glfwPlatformCreateTlsPosix,
    ._glfwPlatformDestroyTlsFunc = _glfwPlatformDestroyTlsPosix,
    ._glfwPlatformGetTlsFunc = _glfwPlatformGetTlsPosix,
    ._glfwPlatformSetTlsFunc = _glfwPlatformSetTlsPosix,

    ._glfwPlatformCreateMutexFunc = _glfwPlatformCreateMutexPosix,
    ._glfwPlatformDestroyMutexFunc = _glfwPlatformDestroyMutexPosix,
    ._glfwPlatformLockMutexFunc = _glfwPlatformLockMutexPosix,
    ._glfwPlatformUnlockMutexFunc = _glfwPlatformUnlockMutexPosix,

    ._glfwPlatformGetTimerValueFunc = _glfwPlatformGetTimerValuePosix,
    ._glfwPlatformGetTimerFrequencyFunc = _glfwPlatformGetTimerFrequencyPosix,
};

