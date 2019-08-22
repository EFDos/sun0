config_data = configuration_data()
config_data.set('codename', version_codename)
config_data.set('version',  version_str)
config_data.set('major',    version_str[0])
config_data.set('minor',    version_str[2])
config_data.set('rev',      version_str[4])

configure_file(
  input: 'version.hpp.in',
  output: 'version.hpp',
  configuration: config_data
)

sun_lib_src = [
  'common/color.cpp',
  'common/time.cpp',
  'math/matrix4.cpp',
  'core/filesys/filesys.cpp',
  'core/filesys/input_stream.cpp',
  'core/logger.cpp',
  'core/clock.cpp',
  'core/context.cpp',
  'core/application.cpp',
  'core/window.cpp',
  'core/input/keyboard.cpp',
  #'core/input/input_map.cpp',
  'system/system.cpp',
  'resources/resource.cpp',
  'resources/resource_cache.cpp',
  'audio/decoders/vorbis_decoder.cpp',
  'audio/audio_server.cpp',
  'audio/sound_source.cpp',
  'audio/sound_buffer.cpp',
  'audio/sound_stream.cpp',
  'physics/physics_server.cpp',
  'physics/physics_rasterizer.cpp',
  'physics/contact_listener.cpp',
  'physics/rigid_body.cpp',
  'graphics/font.cpp',
  'graphics/image.cpp',
  'graphics/camera.cpp',
  'graphics/sprite.cpp',
  'graphics/sprite_batch.cpp',
  'graphics/text.cpp',
  'graphics/shape2D.cpp',
  'graphics/light2D.cpp',
  'graphics/renderer.cpp',
  'graphics/shader.cpp',
  'graphics/vertex_buffer.cpp',
  'graphics/index_buffer.cpp',
  'graphics/texture.cpp',
  'graphics/framebuffer.cpp',
  'graphics/opengl/renderer.cpp',
  'graphics/opengl/shader.cpp',
  'graphics/opengl/vertex_buffer.cpp',
  'graphics/opengl/index_buffer.cpp',
  'graphics/opengl/texture.cpp',
  'graphics/opengl/framebuffer.cpp',
  'gui/gui_system.cpp',
  'gui/widget.cpp',
  'gui/frame.cpp',
  'gui/bar.cpp',
  'gui/text_button.cpp',
  'scene/scene_manager.cpp',
  'scene/scene_tree.cpp',
  'scene/animation.cpp',
  'scene/entity.cpp',
  'script/script_context.cpp',
  'script/script.cpp',
  'script/script_api.cpp',
]

if get_option('dynamic-lib')
  sun_lib = shared_library('sun-engine', sun_lib_src,
    dependencies: sun_depends,
    include_directories: sun_includes,
    cpp_pch: ['pch/stdpch.h']
  )
else
  sun_lib = static_library('sun-engine', sun_lib_src,
    dependencies: sun_depends,
    include_directories: sun_includes,
    cpp_pch: ['pch/stdpch.h', 'pch/solpch.h']
  )
endif
