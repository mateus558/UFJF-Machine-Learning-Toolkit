project(
  'ufjfmltk-classifier',
  'cpp',
  version : '0.40.0',
  default_options : ['warning_level=0', 'cpp_std=c++17']
)

cpp_compiler = meson.get_compiler('cpp')
project_dependencies = [
    cpp_compiler.find_library('ufjfmltk-core', required : true),
]

project_description = 'UFJF-MLTK classifier library.'
prefix = get_option('prefix')
project_subdir = 'ufjfmltk/classifier'
project_install_dir = get_option('libdir')
message('Install directory: ' + prefix / project_install_dir)
project_source_files = ['src/IMA.cpp', 'src/Perceptron.cpp', 'src/SMO.cpp']
project_headers = ['include/Classifier.hpp', 
    'include/DualClassifier.hpp', 
    'include/IMA.hpp', 
    'include/KNNClassifier.hpp',
    'include/OneVsAll.hpp',
    'include/OneVsOne.hpp',
    'include/Perceptron.hpp',
    'include/PrimalClassifier.hpp',
    'include/SMO.hpp',
    'include/hnswlib/bruteforce.h',
    'include/hnswlib/hnswalg.h',
    'include/hnswlib/hnswlib.h',
    'include/hnswlib/space_ip.h',
    'include/hnswlib/space_l2.h',
    'include/hnswlib/visited_list_pool.h']

public_headers = include_directories('include')

project_target = library(meson.project_name(), [project_source_files, project_headers],
  install : true,
  install_dir : project_install_dir,
  include_directories : public_headers,
  version: meson.project_version(),
  soversion: '0',
  dependencies: project_dependencies
)

install_headers('Classifier.hpp', subdir: 'ufjfmltk')
install_headers(project_headers, subdir : project_subdir)

project_target_dep = declare_dependency(
  include_directories: [include_directories('include'), include_directories('src')],
  sources: project_headers,
  link_with : project_target,
)

varprefix = meson.project_name().split('-')
set_variable('_'.join(varprefix) + '_dep', project_target_dep)
set_variable('_'.join(varprefix) + '_tgt', project_target)

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : meson.project_name(),
  filebase : meson.project_name(),
  description : project_description,
  subdirs : project_subdir,
)