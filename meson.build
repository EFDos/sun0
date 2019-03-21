project('sun0', ['cpp', 'c'],
  version: '0.5.0',
  meson_version: '>= 0.43.0',
  license: 'GPL3',
  default_options: 'cpp_std=c++17'
)

version_codename = 'Sunny Alpha'
version_str = meson.project_version()

sun_includes = include_directories(
  './src/thirdparty',
  './src/engine',
  '/usr/include/freetype2'
)

sun_depends = [
	dependency('sdl2'),
	dependency('glew'),
	dependency('openal'),
	dependency('vorbis'),
	meson.get_compiler('cpp').find_library('freetype')
]

subdir('src/engine')
if get_option('build-demos')
  subdir('src/demos')
endif

