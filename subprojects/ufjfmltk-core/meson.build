project(
  'ufjfmltk-core',
  'cpp',
  version : '0.40.0',
  default_options : ['warning_level=0', 'cpp_std=c++17']
)

project_description = 'UFJF-MLTK core library.'
prefix = get_option('prefix')
project_subdir = 'ufjfmltk/core'
project_install_dir = get_option('libdir')
message('Install directory: ' + prefix / project_install_dir)
project_source_files = ['src/Utils.cpp']
project_headers = ['include/ufjfmltk/core/Data.hpp',
    'include/ufjfmltk/core/Point.hpp',
    'include/ufjfmltk/core/DistanceMetric.hpp',
    'include/ufjfmltk/core/Kernel.hpp',
    'include/ufjfmltk/core/Learner.hpp',
    'include/ufjfmltk/core/Point.hpp',
    'include/ufjfmltk/core/Random.hpp',
    'include/ufjfmltk/core/Sampling.hpp',
    'include/ufjfmltk/core/Solution.hpp',
    'include/ufjfmltk/core/Statistics.hpp',
    'include/ufjfmltk/core/Timer.hpp',
    'include/ufjfmltk/core/Utils.hpp',
    'include/ufjfmltk/core/ExprOps.hpp',
    'include/ufjfmltk/core/ExprScalar.hpp',
    'include/ufjfmltk/core/ExprTraits.hpp',
    'include/ufjfmltk/core/Datasets.hpp'
    ]

public_headers = include_directories('include')
lib_args = []

project_target = library(meson.project_name(), [project_source_files, project_headers],
  install : true,
  install_dir : project_install_dir,
  include_directories : public_headers,
  version: meson.project_version(),
  soversion: '0',
)

install_headers('include/ufjfmltk/Core.hpp', subdir: 'ufjfmltk')
install_headers(project_headers, subdir : project_subdir)

project_target_dep = declare_dependency(
  include_directories: [include_directories('include','src','.')],
  sources: project_headers,
  link_with : project_target,
)

meson.override_dependency('ufjfmltk-core', project_target_dep)

varprefix = '_'.join(meson.project_name().split('-'))
set_variable(varprefix + '_tgt', project_target)

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : meson.project_name(),
  filebase : meson.project_name(),
  description : project_description,
  libraries : project_target,
  subdirs : project_subdir,
  version : meson.project_version()
)