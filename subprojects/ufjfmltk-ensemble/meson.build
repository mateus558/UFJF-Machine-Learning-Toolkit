project('ufjfmltk-ensemble', 'cpp',
  version : '1.0.0',
  default_options : ['warning_level=0', 'cpp_std=c++17'])

cpp_compiler = meson.get_compiler('cpp')
project_dependencies = [
    cpp_compiler.find_library('ufjfmltk-core', required : true),
    cpp_compiler.find_library('ufjfmltk-classifier', required : true)
]

project_description = 'UFJF-MLTK ensemble library.'
prefix = get_option('prefix')
project_subdir = 'ufjfmltk/ensemble'
project_install_dir = get_option('libdir')
message('Install directory: ' + prefix / project_install_dir)
project_headers = ['include/AdaBoostClassifier.hpp', 
    'include/AutoWeightedVoting.hpp', 
    'include/BaggingClassifier.hpp', 
    'include/Ensemble.hpp',
    'include/kNNEnsemble.hpp',
    'include/kNNEnsembleBagging.hpp',
    'include/kNNEnsembleDSM.hpp',
    'include/kNNEnsembleRSM.hpp',
    'include/kNNEnsembleW.hpp',
    'include/kNNEnsembleWPDSM.hpp',
    'include/kNNEnsembleWPDSMLinear.hpp',
    'include/kNNEnsembleWRSM.hpp',
    'include/kNNEnsembleWSS.hpp',
    'include/PerceptronCommittee.hpp',
    'include/VotingClassifier.hpp']

public_headers = include_directories('include')

project_target = library(meson.project_name(), [project_headers],
  install : true,
  install_dir : project_install_dir,
  include_directories : public_headers,
  version: meson.project_version(),
  soversion: '0',
  dependencies: project_dependencies
)

install_headers('Ensemble.hpp', subdir: 'ufjfmltk')
install_headers(project_headers, subdir : project_subdir)

project_target_dep = declare_dependency(
  include_directories: [include_directories('include')],
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