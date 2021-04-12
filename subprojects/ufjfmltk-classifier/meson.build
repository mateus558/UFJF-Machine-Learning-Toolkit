project(
  'ufjfmltk-classifier',
  'cpp',
  version : '0.40.0',
  default_options : ['warning_level=0', 'cpp_std=c++17']
)

project_description = 'UFJF-MLTK classifier library.'
prefix = get_option('prefix')
project_subdir = 'ufjfmltk/classifier'
project_install_dir = get_option('libdir')
message('Install directory: ' + prefix / project_install_dir)
project_source_files = ['src/IMA.cpp', 'src/Perceptron.cpp', 'src/SMO.cpp']
project_headers = ['include/ufjfmltk/classifier/Classifier.hpp', 
    'include/ufjfmltk/classifier/DualClassifier.hpp', 
    'include/ufjfmltk/classifier/IMA.hpp', 
    'include/ufjfmltk/classifier/KNNClassifier.hpp',
    'include/ufjfmltk/classifier/OneVsAll.hpp',
    'include/ufjfmltk/classifier/OneVsOne.hpp',
    'include/ufjfmltk/classifier/Perceptron.hpp',
    'include/ufjfmltk/classifier/PrimalClassifier.hpp',
    'include/ufjfmltk/classifier/SMO.hpp',
    'include/ufjfmltk/classifier/hnswlib/bruteforce.h',
    'include/ufjfmltk/classifier/hnswlib/hnswalg.h',
    'include/ufjfmltk/classifier/hnswlib/hnswlib.h',
    'include/ufjfmltk/classifier/hnswlib/space_ip.h',
    'include/ufjfmltk/classifier/hnswlib/space_l2.h',
    'include/ufjfmltk/classifier/hnswlib/visited_list_pool.h']

project_dependencies = []
if not meson.is_subproject()
  cpp_compiler = meson.get_compiler('cpp')
  project_dependencies += cpp_compiler.find_library('ufjfmltk-core', required : true)
  public_headers = include_directories('include') 
else
  project_dependencies += subproject('ufjfmltk-core').get_variable('ufjfmltk_core_dep')
  public_headers = include_directories('include', '../ufjfmltk-core/include', '../ufjfmltk-core/src')
endif

project_target = library(meson.project_name(), [project_source_files, project_headers],
  install : true,
  install_dir : project_install_dir,
  include_directories : public_headers,
  version: meson.project_version(),
  soversion: '0',
  dependencies: project_dependencies
)

install_headers('include/ufjfmltk/Classifier.hpp', subdir: 'ufjfmltk')
install_headers(project_headers, subdir : project_subdir)

project_target_dep = declare_dependency(
  include_directories: [include_directories('include', 'src', '.')],
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