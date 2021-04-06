project(
  'ufjfmltk_core',
  'cpp',
  version : '1.0.0',
  default_options : ['warning_level=0', 'cpp_std=c++17']
)

project_description = 'UFJF-MLTK core library.'

project_source_files = ['src/Data.cpp', 'src/Kernel.cpp', 'src/Statistics.cpp', 'src/Utils.cpp']
project_headers = ['include/Data.hpp', 
    'include/Point.hpp', 
    'include/DistanceMetric.hpp', 
    'include/Kernel.hpp',
    'include/Learner.hpp',
    'include/Point.hpp',
    'include/Random.hpp',
    'include/Sampling.hpp',
    'include/Solution.hpp',
    'include/Statistics.hpp',
    'include/Timer.hpp',
    'include/Utils.hpp',
    'include/CoverTree.hpp',
    'include/ExprOps.hpp',
    'include/ExprScalar.hpp',
    'include/ExprTraits.hpp']


public_headers = include_directories('include')

install_headers('Core.hpp', subdir: 'ufjfmltk')
install_headers(project_headers, subdir : 'ufjfmltk/core')

project_target = static_library(meson.project_name(), [project_source_files, project_headers],
  install : true,
  include_directories : public_headers
)

project_target_dep = declare_dependency(
  include_directories: include_directories('.'),
  sources: [project_headers],
  link_whole : project_target
)

set_variable(meson.project_name() + '_dep', project_target_dep)
set_variable(meson.project_name() + '_tgt', project_target)

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : meson.project_name(),
  filebase : meson.project_name(),
  description : project_description,
  subdirs : 'ufjfmltk/core',
)
