project('sun0', ['cpp', 'c'],
  version: '0.1.3',
  meson_version: '>= 0.43.0',
  license: 'GPL3',
  default_options: 'cpp_std=c++17'
)

version_codename = 'Sunny Alpha'
version_str = meson.project_version()

sun_includes = include_directories('./include', './src/engine')

sun_depends = [
	dependency('sdl2'),
	dependency('glew')
]

subdir('src/engine')
if get_option('build-demos')
  subdir('src/demos')
endif

