#--------------------------------------------------------------------
# Generate glfw_config.h
#--------------------------------------------------------------------

config_data = configuration_data()

config_data.set('_GLFW_WAYLAND',        wayland_backend_enabled)
config_data.set('_GLFW_OSMESA',         osmesa_backend_enabled)
config_data.set('_GLFW_WIN32',          win32_backend_enabled)
config_data.set('_GLFW_COCOA',          cocoa_backend_enabled)
config_data.set('_GLFW_GBM',            gbm_backend_enabled)
config_data.set('_GLFW_X11',            x11_backend_enabled)

config_data.set('_GLFW_BUILD_DLL',      not (get_option('default_library') == 'static'))
config_data.set('_GLFW_USE_HYBRID_HPG', get_option('glfw_use_hybrid_hpg').enabled())

config_file = configure_file(configuration: config_data, output: 'glfw_config.h')

#config_data.set('_GLFW_VULKAN_STATIC', get_option('glfw_vulkan_static'))

#option('glfw_build_shared',   type : 'boolean', value : 'false', description : 'Build shared libraries')

#--------------------------------------------------------------------
# Add common sources
#--------------------------------------------------------------------

sources = [
  'context.c',
  'init.c',
  'input.c',
  'monitor.c',
  'vulkan.c',
  'window.c',
]

headers = [
  'internal.h',
  'mappings.h',
  config_file,
  '../include/GLFW/glfw3.h',
  '../include/GLFW/glfw3native.h',
]

dependencies = []

if unix
  dependencies += [rt_dep, math_dep, compiler.find_library('dl', required: false)]
endif

dependencies += threads

#--------------------------------------------------------------------
# Backend sources
#--------------------------------------------------------------------

if wayland_backend_enabled
  sources += [
    'wl_init.c',
    'wl_monitor.c',
    'wl_window.c',
    'posix_time.c',
    'posix_thread.c',
    'xkb_unicode.c',
    'egl_context.c',
    'osmesa_context.c',
  ]

  headers += [
    'wl_platform.h',
    'posix_time.h',
    'posix_thread.h',
    'xkb_unicode.h',
    'egl_context.h',
    'osmesa_context.h'
  ]

  wl_protocol_dir = wayland_protos.get_pkgconfig_variable('pkgdatadir')
  wayland_scanner = dependency('wayland-scanner', required: false, native: true)
  if wayland_scanner.found()
    wayland_scanner_program = find_program(
      wayland_scanner.get_pkgconfig_variable('wayland_scanner'),
      native: true,
    )
  else
    wayland_scanner_program = find_program('wayland-scanner', native: true)
  endif

  protocols = [
    [wl_protocol_dir, 'stable/xdg-shell/xdg-shell.xml'],
    [wl_protocol_dir, 'unstable/xdg-decoration/xdg-decoration-unstable-v1.xml'],
    [wl_protocol_dir, 'stable/viewporter/viewporter.xml'],
    [wl_protocol_dir, 'unstable/relative-pointer/relative-pointer-unstable-v1.xml'],
    [wl_protocol_dir, 'unstable/pointer-constraints/pointer-constraints-unstable-v1.xml'],
    [wl_protocol_dir, 'unstable/idle-inhibit/idle-inhibit-unstable-v1.xml'],
  ]

  wayland_backend_protos_src = []
  wayland_backend_protos_headers = []

  foreach p : protocols
    xml = join_paths(p)
    wayland_backend_protos_src += custom_target(
      xml.underscorify() + '_c',
      input: xml,
      output: '@BASENAME@-protocol.c',
      command: [wayland_scanner_program, 'private-code', '@INPUT@', '@OUTPUT@'],
    )
    wayland_backend_protos_headers += custom_target(
      xml.underscorify() + '_h',
      input: xml,
      output: '@BASENAME@-client-protocol.h',
      command: [wayland_scanner_program, 'client-header', '@INPUT@', '@OUTPUT@']
    )
  endforeach

  sources += wayland_backend_protos_src
  headers += wayland_backend_protos_headers
  dependencies += [wayland_client, wayland_protos, wayland_egl]
endif

if osmesa_backend_enabled
  sources += [
    'null_init.c',
    'null_monitor.c',
    'null_window.c',
    'null_joystick.c',
    'posix_time.c',
    'posix_thread.c',
    'osmesa_context.c',
  ]
  headers += [
    'null_platform.h',
    'null_joystick.h',
    'posix_time.h',
    'posix_thread.h',
    'osmesa_context.h'
  ]
  dependencies += [osmesa]
endif

if win32_backend_enabled
  sources += [
    'win32_init.c',
    'win32_joystick.c',
    'win32_monitor.c',
    'win32_time.c',
    'win32_thread.c',
    'win32_window.c',
    'wgl_context.c',
    'egl_context.c',
    'osmesa_context.c'
  ]

  headers += [
    'win32_platform.h',
    'win32_joystick.h',
    'wgl_context.h',
    'egl_context.h',
    'osmesa_context.h'
  ]
endif

if cocoa_backend_enabled
  sources += [
    'cocoa_init.m',
    'cocoa_joystick.m',
    'cocoa_monitor.m',
    'cocoa_window.m',
    'cocoa_time.c',
    'posix_thread.c',
    'nsgl_context.m',
    'egl_context.c',
    'osmesa_context.c',
  ]

  headers += [
    'cocoa_platform.h',
    'cocoa_joystick.h',
    'posix_thread.h',
    'nsgl_context.h',
    'egl_context.h',
    'osmesa_context.h',
  ]

endif

if gbm_backend_enabled
endif

if x11_backend_enabled
  sources += [
    'x11_init.c',
    'x11_monitor.c',
    'x11_window.c',
    'xkb_unicode.c',
    'posix_time.c',
    'posix_thread.c',
    'glx_context.c',
    'egl_context.c',
    'osmesa_context.c',
  ]

  headers += [
    'x11_platform.h',
    'xkb_unicode.h',
    'posix_time.h',
    'posix_thread.h',
    'glx_context.h',
    'egl_context.h',
    'osmesa_context.h'
  ]
  
  dependencies += [x11_dep, xrandr_dep, xinerama_dep, xkb_dep, xcursor_dep]
endif

if x11_backend_enabled or wayland_backend_enabled or gbm_backend_enabled
  if os == 'linux'
    headers += ['linux_joystick.h']
    sources += ['linux_joystick.c']
  else
    headers += ['null_joystick.h']
    sources += ['null_joystick.c']
  endif
endif

#--------------------------------------------------------------------
# Library targets
#--------------------------------------------------------------------

glfw_library = library(
  glfw_library_name,
  sources + headers,
  c_args: ['-D_GLFW_USE_CONFIG_H'],
  dependencies: dependencies,
  include_directories: [
    include_directories('../include'),
    include_directories('.')
  ]
)

glfw_dep = declare_dependency(
  link_with: glfw_library,
  include_directories: [
    include_directories('../include'),
    include_directories('.')
  ]
)

#--------------------------------------------------------------------
# Generate pkg-config file
#--------------------------------------------------------------------

pkgconfig.generate(
  glfw_library,
  name: 'GLFW',
  filebase: glfw_library_name,
  description: 'A multi-platform library for OpenGL, Vulkan, window and input',
  url: 'https://www.glfw.org',
)
