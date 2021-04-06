project(
  'ufjfmltk_core',
  'cpp',
  version : '1.0.0',
  default_options : ['warning_level=0', 'cpp_std=c++17']
)

project_description = 'UFJF-MLTK core library.'

project_source_files = ['src/gnuplot_i.cpp', 'src/Visualization.cpp']
project_headers = ['include/gnuplot_i.hpp', 
    'include/Visualization.hpp']

project_dependencies = []

gnuplot_dep = dependency('gnuplot5', required : false)
libcore_dep = dependency('ufjfmltk_core', required : true)

if not libcore_dep.found()
  libcore_proj = subproject('ufjfmltk_core')
  libcore_dep = libcore_proj.get_variable('ufjfmltk_core_dep')
endif

project_dependencies += gnuplot_dep
project_dependencies += libcore_dep

public_headers = include_directories('include')

install_headers('Visualization.hpp', subdir: 'ufjfmltk')
install_headers(project_headers, subdir : 'ufjfmltk/visual')

project_target = static_library(
  meson.project_name(),
  project_source_files,
  install : true,
  include_directories : public_headers,
  dependencies : project_dependencies
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
  libraries : project_target,
)
