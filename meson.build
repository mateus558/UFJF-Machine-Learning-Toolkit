project('ufjfmltk', 'cpp',
  version : '0.40.0',
  default_options : ['warning_level=0', 'cpp_std=c++17','default_library=shared'])

project_description = 'UFJF Machine Learning Toolkit'
prefix = get_option('prefix')
project_subdir = 'ufjfmltk'
project_install_dir = get_option('libdir')
build_type = get_option('buildtype')
library_type = get_option('default_library')

message('Build type: ' + build_type)
message('Install directory: ' + prefix / project_install_dir)
message('Default library type: ' + library_type)
message('\nInstalling the necessary submodules, this can take some time. \n')

if target_machine.system() == 'linux'
  message('Installing ufjfmltk-core...')
  run_command('./module_installer.sh', 'ufjfmltk-core', prefix)
  message('Installing ufjfmltk-classifier...')
  run_command('./module_installer.sh', 'ufjfmltk-classifier', prefix)
  message('Installing ufjfmltk-regressor...')
  run_command('./module_installer.sh', 'ufjfmltk-regressor', prefix)
  message('Installing ufjfmltk-valid...')
  run_command('./module_installer.sh', 'ufjfmltk-valid', prefix)
  message('Installing ufjfmltk-ensemble...')
  run_command('./module_installer.sh', 'ufjfmltk-ensemble', prefix)
  message('Installing ufjfmltk-featselect...')
  run_command('./module_installer.sh', 'ufjfmltk-featselect', prefix)
  message('Installing ufjfmltk-clusterer...')
  run_command('./module_installer.sh', 'ufjfmltk-clusterer', prefix)
  message('Installing ufjfmltk-visual...')
  run_command('./module_installer.sh', 'ufjfmltk-visual', prefix)
endif

modules = {
  'core': subproject('ufjfmltk-core'),
  'classifier': subproject('ufjfmltk-classifier'),
  'regressor': subproject('ufjfmltk-regressor'),
  'valid': subproject('ufjfmltk-valid'),
  'ensemble': subproject('ufjfmltk-ensemble'),
  'featselect': subproject('ufjfmltk-featselect'),
  'clusterer': subproject('ufjfmltk-clusterer'),
  'visual': subproject('ufjfmltk-visual')
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
  modules['core'].get_variable('ufjfmltk_core_dep'),
  modules['classifier'].get_variable('ufjfmltk_classifier_dep'),
  modules['valid'].get_variable('ufjfmltk_valid_dep'),
  modules['ensemble'].get_variable('ufjfmltk_ensemble_dep'),
  modules['featselect'].get_variable('ufjfmltk_featselect_dep'),
  modules['regressor'].get_variable('ufjfmltk_regressor_dep'),
  modules['clusterer'].get_variable('ufjfmltk_clusterer_dep'),
  modules['visual'].get_variable('ufjfmltk_visual_dep')
]

install_headers('ufjfmltk.hpp', subdir: 'ufjfmltk')

project_target = library(meson.project_name(), [],
  dependencies: project_dependencies,
  objects: combined,
  install : true,
  install_dir : project_install_dir
)

project_target_dep = declare_dependency(
  link_with : project_target,
  include_directories : include_directories('.'),
  dependencies: project_dependencies
)

set_variable(meson.project_name() + '_dep', project_target_dep)
set_variable(meson.project_name() + '_tgt', project_target)

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : meson.project_name(),
  filebase : meson.project_name(),
  description : project_description,
  subdirs : project_subdir
)