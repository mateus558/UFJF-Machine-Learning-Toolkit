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

public_headers = include_directories('include', is_system:true)
lib_args = ['-DBUILDING_CORE']

project_target = library(meson.project_name(), [project_source_files, project_headers],
  install : true,
  install_dir : project_install_dir,
  include_directories : public_headers,
  version: meson.project_version(),
  soversion: '0',
  cpp_args : lib_args,
  gnu_symbol_visibility : 'default',
)

install_headers('Core.hpp', subdir: 'ufjfmltk')
install_headers(project_headers, subdir : project_subdir)

project_target_dep = declare_dependency(
  include_directories: [include_directories('include','src','.', is_system : true)],
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
  libraries : project_target,
  subdirs : project_subdir,
  version : meson.project_version()
)