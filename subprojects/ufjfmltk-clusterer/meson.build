project(
  'ufjfmltk-clusterer',
  'cpp',
  version : '0.40.0',
  default_options : ['warning_level=0', 'cpp_std=c++17']
)

project_description = 'UFJF-MLTK clustering library.'
prefix = get_option('prefix')
project_subdir = 'ufjfmltk/clusterer'
project_install_dir = get_option('libdir')
message('Install directory: ' + prefix / project_install_dir)
project_source_files = []
project_headers = ['include/ufjfmltk/clusterer/Clusterer.hpp',
    'include/ufjfmltk/clusterer/KMeans.hpp']

project_dependencies = []

project_dependencies += dependency('ufjfmltk-core')
public_headers = include_directories('include')

project_target = library(meson.project_name(), [project_source_files, project_headers],
  install : true,
  install_dir : project_install_dir,
  include_directories : public_headers,
  version: meson.project_version(),
  soversion: '0',
  dependencies: project_dependencies
)

install_headers('include/ufjfmltk/Clusterer.hpp', subdir: 'ufjfmltk')
install_headers(project_headers, subdir : project_subdir)

project_target_dep = declare_dependency(
  include_directories: [include_directories('include','.')],
  sources: project_headers,
  link_with : project_target,
)

meson.override_dependency('ufjfmltk-clusterer', project_target_dep)

varprefix = '_'.join(meson.project_name().split('-'))
set_variable(varprefix + '_tgt', project_target)

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : meson.project_name(),
  filebase : meson.project_name(),
  description : project_description,
  subdirs : project_subdir,
)
