project(
  'ufjfmltk-visual',
  'cpp',
  version : '0.40.0',
  default_options : ['warning_level=0', 'cpp_std=c++17']
)

cpp_compiler = meson.get_compiler('cpp')
project_dependencies = [
    cpp_compiler.find_library('ufjfmltk-core', required : true)
]

project_description = 'UFJF-MLTK visualization library.'
prefix = get_option('prefix')
project_subdir = 'ufjfmltk/visual'
project_install_dir = get_option('libdir')
project_source_files = ['src/gnuplot_i.cpp', 'src/Visualization.cpp']
project_headers = ['include/gnuplot_i.hpp', 
    'include/Visualization.hpp']

public_headers = include_directories('include')

project_target = library(meson.project_name(), [project_headers, project_source_files],
  install : true,
  install_dir : project_install_dir,
  include_directories : public_headers,
  version: meson.project_version(),
  soversion: '0',
  dependencies: project_dependencies
)

install_headers('Visualize.hpp', subdir: 'ufjfmltk')
install_headers(project_headers, subdir : project_subdir)

project_target_dep = declare_dependency(
  include_directories: [include_directories('include','src','.')],
  sources: project_headers,
  link_with : project_target,
)

varprefix = '_'.join(meson.project_name().split('-'))
set_variable(varprefix + '_dep', project_target_dep)
set_variable(varprefix + '_tgt', project_target)

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : meson.project_name(),
  filebase : meson.project_name(),
  description : project_description,
  subdirs : project_subdir,
)