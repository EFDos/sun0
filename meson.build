project('sun0', ['cpp', 'c'],
  version: '0.7.5',
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
	dependency('vorbisfile'),
	dependency('threads'),
	meson.get_compiler('cpp').find_library('freetype'),
	meson.get_compiler('cpp').find_library('Box2D',
        dirs: join_paths(meson.source_root(), 'src/thirdparty/Box2D')),
]

subdir('src/engine')
if get_option('build-demos')
  subdir('src/demos')
endif

