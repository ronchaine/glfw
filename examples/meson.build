examples_linker_args = []
examples_dependencies = [glfw_dep, math_dep]

if compiler.get_id() == 'msvc'
  tests_linker_args += '/ENTRY:mainCRTStartup'
endif

glad_gl = ['../deps/glad/gl.h',
           '../deps/glad_gl.c']

glad_vulkan = ['../deps/glad/vulkan.h',
               '../deps/glad_vulkan.c']

getopt = ['../deps/getopt.h',
          '../deps/getopt.c']

tinycthread = ['../deps/tinycthread.h',
               '../deps/tinycthread.c']

os_extra_files = []

if os == 'windows'
  winrc = import('windows')

  win_extras = winrc.compile_resources(
    'glfw.ico'
  )

  #os_extra_files += win_extras
endif

programs = [
  ['boing',           [['boing.c']            + glad_gl]],
  ['gears',           [['gears.c']            + glad_gl]],
  ['heightmap',       [['heightmap.c']        + glad_gl]],
  ['offscreen',       [['offscreen.c']        + glad_gl]],
  ['particles',       [['particles.c']        + glad_gl + getopt + tinycthread]],
  ['sharing',         [['sharing.c']          + glad_gl]],
  ['splitview',       [['splitview.c']        + glad_gl]],
  ['triangle-opengl', [['triangle-opengl.c']  + glad_gl]],
  ['wave',            [['wave.c']             + glad_gl]],
]

foreach example_program : programs
  executable(
    example_program[0],
    example_program[1],
    link_args: examples_linker_args,
    dependencies: examples_dependencies,
    include_directories: include_directories('../deps'),
  )
endforeach

