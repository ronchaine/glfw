tests_linker_args = []
tests_dependencies = [glfw_dependency, math_dep]

if compiler.get_id() == 'msvc'
  tests_linker_args += '/ENTRY:mainCRTStartup'
endif

glad_gl = [meson.source_root() + '/deps/glad/gl.h',
           meson.source_root() + '/deps/glad_gl.c']

glad_vulkan = [meson.source_root() + '/deps/glad/vulkan.h',
               meson.source_root() + '/deps/glad_vulkan.c']

getopt = [meson.source_root() + '/deps/getopt.h',
          meson.source_root() + '/deps/getopt.c']

tinycthread = [meson.source_root() + '/deps/tinycthread.h',
               meson.source_root() + '/deps/tinycthread.c']

programs = [
  ['clipboard', [['clipboard.c']  + glad_gl + getopt]],
  ['events',    [['events.c']     + glad_gl + getopt]],
  ['msaa',      [['msaa.c']       + glad_gl + getopt]],
  ['glfwinfo',  [['glfwinfo.c']   + glad_gl + glad_vulkan + getopt]],
  ['iconify',   [['iconify.c']    + glad_gl + getopt]],
  ['monitors',  [['monitors.c']   + glad_gl + getopt]],
  ['reopen',    [['reopen.c']     + glad_gl]],
  ['cursor',    [['cursor.c']     + glad_gl]],

  ['empty',     [['empty.c']      + glad_gl + tinycthread]],
  ['gamma',     [['gamma.c']      + glad_gl]],
  ['icon',      [['icon.c']       + glad_gl]],
  ['inputlag',  [['inputlag.c']   + glad_gl + getopt]],
  ['joysticks', [['joysticks.c']  + glad_gl]],
  ['opacity',   [['opacity.c']    + glad_gl]],
  ['tearing',   [['tearing.c']    + glad_gl]],
  ['threads',   [['threads.c']    + glad_gl + tinycthread]],
  ['timeout',   [['timeout.c']    + glad_gl]],
  ['title',     [['title.c']      + glad_gl]],
  ['vulkan',    [['vulkan.c']     + glad_vulkan]],
  ['windows',   [['windows.c']    + glad_gl + getopt]]
]

foreach test_program : programs
  executable(
    test_program[0],
    test_program[1],
    link_args: tests_linker_args,
    dependencies: tests_dependencies,
    include_directories: include_directories('../deps'),
  )
endforeach
