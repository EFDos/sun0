hello_exec = executable('sun-hello', ['hello/hello.cpp'],
  include_directories: sun_includes,
  link_with: sun_lib,
  install: true
)

pong_exec = executable('sun-pong', ['pong/pong.cpp'],
  include_directories: sun_includes,
  link_with: sun_lib,
  install: true
)

sandbox_exec = executable('sun-sandbox', ['sandbox/sandbox.cpp'],
  include_directories: sun_includes,
  link_with: sun_lib,
  cpp_pch: ['../engine/pch/stdpch.h'],
  install: true
)

balls_exec = executable('sun-balls', ['balls/balls.cpp'],
  include_directories: sun_includes,
  link_with: sun_lib,
  install: true
)

gui_exec = executable('sun-gui', ['gui/gui.cpp'],
  include_directories: sun_includes,
  link_with: sun_lib,
  install: true
)
