project(
  'ufjfmltk_regressor',
  'cpp',
  version : '1.0.0',
  default_options : ['warning_level=0', 'cpp_std=c++17']
)

project_dependencies = [
]

core_dep = dependency('ufjfmltk_core', fallback : ['ufjfmltk_core', 'ufjfmltk_core_dep'])

project_dependencies += core_dep

build_args = []
project_description = 'UFJF-MLTK regression library.'

project_source_files = ['src/KNNRegressor.cpp', 'src/LMS.cpp']
project_headers = ['include/Regressor.hpp', 
    'include/DualRegressor.hpp', 
    'include/KNNRegressor.hpp', 
    'include/LMS.hpp',
    'include/PrimalRegressor.hpp']

public_headers = include_directories('include')

build_args = [
]

install_headers('Regressor.hpp', subdir: 'ufjfmltk')
install_headers(project_headers, subdir : 'ufjfmltk/regressor')

project_target = static_library(
  meson.project_name(),
  project_source_files,
  dependencies: core_dep,
  install : true,
  include_directories : public_headers,
)

project_target_dep = declare_dependency(
  include_directories: public_headers,
  link_with : project_target
)

set_variable(meson.project_name() + '_dep', project_target_dep)
set_variable(meson.project_name() + '_tgt', project_target)

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : meson.project_name(),
  filebase : meson.project_name(),
  description : project_description,
  subdirs : 'ufjfmltk/regressor',
)
