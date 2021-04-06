project('ufjfmltk_ensemble', 'cpp',
  version : '1.0.0',
  default_options : ['warning_level=0', 'cpp_std=c++17'])
project_description = 'UFJF-MLTK ensemble library.'

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

project_target = static_library(
  meson.project_name(),
  [],
  install : true,
  include_directories : public_headers,
)


project_dep = declare_dependency(
  include_directories: public_headers
)
set_variable(meson.project_name() + '_dep', project_dep)

# Make this library usable from the system's
# package manager.
install_headers(project_headers, subdir : 'ufjfmltk/ensemble')
install_headers('Ensemble.hpp', subdir : 'ufjfmltk')

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : meson.project_name(),
  filebase : meson.project_name(),
  description : project_description,
  subdirs : 'ufjfmltk/ensemble'
)