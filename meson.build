project('sun0', ['cpp', 'c'],
  version: '1.2.0',
  meson_version: '>= 0.43.0',
  license: 'GPL3',
  default_options: 'cpp_std=c++17'
)

version_codename = 'Summer Beta'
version_str = meson.project_version()

system = host_machine.system()
message('Detected OS: ' + system)

sun_includes = []
box2d_dir = ''

if system == 'linux'
  sun_includes = include_directories(
    './src/thirdparty',
    './src/engine',
    '/usr/include/freetype2',
    '/usr/include/luajit-2.1'
  )
  box2d_dir = join_paths(meson.current_source_dir(), 'src/thirdparty/linux')
endif

if system == 'haiku'
  sun_includes = include_directories(
    './src/thirdparty',
    './src/engine',
    '/boot/system/develop/headers',
    '/boot/system/develop/headers/freetype2',
  )
  box2d_dir = join_paths(meson.current_source_dir(), '/src/thirdparty/haiku/Box2D')
endif

message('source root is: ' + meson.current_source_dir())

sun_depends = [
	dependency('sdl2'),
	dependency('glew'),
	dependency('openal'),
	dependency('vorbisfile'),
	dependency('threads'),
	meson.get_compiler('cpp').find_library('luajit-5.1'),
	meson.get_compiler('cpp').find_library('freetype'),
	meson.get_compiler('cpp').find_library('Box2D', dirs: box2d_dir),
]

subdir('src/engine')
if get_option('build-demos')
  subdir('src/demos')
endif
