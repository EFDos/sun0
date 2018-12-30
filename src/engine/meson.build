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
  'math/matrix4.cpp',
  'core/logger.cpp',
  'core/application.cpp',
  'core/window.cpp'
]

if get_option('dynamic-lib')
  sun_lib = shared_library('sun-engine', sun_lib_src,
    dependencies: sun_depends,
    include_directories: sun_includes
  )
else
  sun_lib = static_library('sun-engine', sun_lib_src,
    dependencies: sun_depends,
    include_directories: sun_includes
  )
endif
