project('glfw', 'c',
  version: '3.4.0',
  license: 'zlib',
  default_options : [
    'buildtype=debugoptimized',
    'default_library=static',
    'b_pie=true',
  ],
)

add_global_arguments('-DMESON_BUILD', language: 'c')

compiler  = meson.get_compiler('c')
os        = host_machine.system()

doxygen   = dependency('doxygen', required: get_option('glfw_build_docs'))
threads   = dependency('threads')

math_dep  = compiler.find_library('m', required: false)
rt_dep    = compiler.find_library('rt', required: false)

pkgconfig = import('pkgconfig')

unix = not ['windows', 'darwin', 'android', 'haiku'].contains(os)

if unix
  glfw_library_name = 'glfw'
else
  glfw_library_name = 'glfw3'
endif

#--------------------------------------------------------------------
# Backends
#--------------------------------------------------------------------

wayland_backend_enabled = false
osmesa_backend_enabled  = false
win32_backend_enabled   = false
cocoa_backend_enabled   = false
gbm_backend_enabled     = false
x11_backend_enabled     = false

wayland_client    = dependency('wayland-client')
wayland_protos    = dependency('wayland-protocols', version : '>=1.15')
wayland_egl       = dependency('wayland-egl')

if wayland_client.found() and wayland_protos.found() and wayland_egl.found()
  wayland_backend_enabled = true
  message('wayland backend enabled')
endif


osmesa_dep          = dependency('osmesa',    required: get_option('glfw_osmesa'))

if osmesa_dep.found()
  osmesa_backend_enabled = true
  message('osmesa backend enabled')
endif


egl_dep             = dependency('egl',       required: get_option('glfw_gbm'))
gbm_dep             = dependency('gbm',       required: get_option('glfw_gbm'))
libdrm_dep          = dependency('libdrm',    required: get_option('glfw_gbm'))

if egl_dep.found() and gbm_dep.found() and libdrm_dep.found()
  gbm_backend_enabled = true
  message('KMS/GBM backend enabled')
endif


x11_dep             = dependency('x11',       required: get_option('glfw_x11'))
xrandr_dep          = dependency('xrandr',    required: get_option('glfw_x11'))
xinerama_dep        = dependency('xinerama',  required: get_option('glfw_x11'))
xkb_dep             = dependency('xcb-xkb',   required: get_option('glfw_x11'))
xcursor_dep         = dependency('xcursor',   required: get_option('glfw_x11'))

if x11_dep.found() and xrandr_dep.found() and xinerama_dep.found() and xkb_dep.found() and xcursor_dep.found()
  x11_backend_enabled = true
  message('x11 backend enabled')
endif

if os == 'darwin' and not get_option('glfw_cocoa').disabled()
  cocoa_backend_enabled = true
  message('cocoa backend enabled')
endif


if os == 'windows' and not get_option('glfw_win32').disabled()
  win32_backend_enabled = true
  message('win32 backend enabled')
endif

#--------------------------------------------------------------------
# Set compiler specific flags
#--------------------------------------------------------------------

#--------------------------------------------------------------------
# Add subdirectories
#--------------------------------------------------------------------

subdir('src')

#if not get_option('glfw_build_examples').disabled()
#  subdir('examples')
#endif
#

if not get_option('glfw_build_tests').disabled()
  subdir('tests')
endif

#if doxygen.found()
#  subdir('docs')
#endif

