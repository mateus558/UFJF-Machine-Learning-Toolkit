project('ufjfmltk', 'cpp',
  version : '1.0.0',
  default_options : ['warning_level=0', 'cpp_std=c++17'])

prefix = get_option('prefix')
message('Prefix: '+prefix)
message('Installing the necessary submodules, this can take some time. \n')
if target_machine.system() == 'linux'
  message('Installing ufjfmltk_core...')
  run_command('./module_installer.sh', 'ufjfmltk_core')
  message('Installing ufjfmltk_classifier...')
  run_command('./module_installer.sh', 'ufjfmltk_classifier')
  message('Installing ufjfmltk_regressor...')
  run_command('./module_installer.sh', 'ufjfmltk_regressor')
  message('Installing ufjfmltk_valid...')
  run_command('./module_installer.sh', 'ufjfmltk_valid')
  message('Installing ufjfmltk_ensemble...')
  run_command('./module_installer.sh', 'ufjfmltk_ensemble')
  message('Installing ufjfmltk_featselect...')
  run_command('./module_installer.sh', 'ufjfmltk_featselect')
  message('Installing ufjfmltk_clusterer...')
  run_command('./module_installer.sh', 'ufjfmltk_clusterer')
  message('Installing ufjfmltk_visual...')
  run_command('./module_installer.sh', 'ufjfmltk_visual')
endif

project_description = 'UFJF Machine Learning Toolkit'

modules = {
  'core': subproject('ufjfmltk_core'),
  'classifier': subproject('ufjfmltk_classifier'),
  'regressor': subproject('ufjfmltk_regressor'),
  'valid': subproject('ufjfmltk_valid'),
  'ensemble': subproject('ufjfmltk_ensemble'),
  'featselect': subproject('ufjfmltk_featselect'),
  'clusterer': subproject('ufjfmltk_clusterer'),
  'visual': subproject('ufjfmltk_visual')
}

combined = [
  modules['core'].get_variable('ufjfmltk_core_tgt').extract_all_objects(recursive:true),
  modules['classifier'].get_variable('ufjfmltk_classifier_tgt').extract_all_objects(recursive:true),
  modules['valid'].get_variable('ufjfmltk_valid_tgt').extract_all_objects(recursive:true),
  modules['ensemble'].get_variable('ufjfmltk_ensemble_tgt').extract_all_objects(recursive:true),
  modules['featselect'].get_variable('ufjfmltk_featselect_tgt').extract_all_objects(recursive:true),
  modules['regressor'].get_variable('ufjfmltk_regressor_tgt').extract_all_objects(recursive:true),
  modules['clusterer'].get_variable('ufjfmltk_clusterer_tgt').extract_all_objects(recursive:true),
  modules['visual'].get_variable('ufjfmltk_visual_tgt').extract_all_objects(recursive:true)
]

project_dependencies = [
  dependency('ufjfmltk_core', fallback : ['ufjfmltk_core', 'ufjfmltk_core_dep']),
  dependency('ufjfmltk_classifier', fallback : ['ufjfmltk_classifier', 'ufjfmltk_classifier_dep']),
  dependency('ufjfmltk_regressor', fallback : ['ufjfmltk_regressor', 'ufjfmltk_regressor_dep']),
  dependency('ufjfmltk_valid', fallback : ['ufjfmltk_valid', 'ufjfmltk_valid_dep']),
  dependency('ufjfmltk_ensemble', fallback : ['ufjfmltk_ensemble', 'ufjfmltk_ensemble_dep']),
  dependency('ufjfmltk_featselect', fallback : ['ufjfmltk_featselect', 'ufjfmltk_featselect_dep']),
  dependency('ufjfmltk_clusterer', fallback : ['ufjfmltk_clusterer', 'ufjfmltk_clusterer_dep']),
  dependency('ufjfmltk_visual', fallback : ['ufjfmltk_visual', 'ufjfmltk_visual_dep'])
]

install_headers('ufjfmltk.hpp', subdir: 'ufjfmltk')

project_target = static_library(meson.project_name(), [],
  dependencies: project_dependencies,
  objects: combined,
  install : true,
)

project_target_dep = declare_dependency(
  link_whole : project_target,
  include_directories : include_directories('.'),
  dependencies: project_dependencies
)

set_variable(meson.project_name() + '_dep', project_target_dep)

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : meson.project_name(),
  filebase : meson.project_name(),
  description : project_description,
  subdirs : 'ufjfmltk'
)