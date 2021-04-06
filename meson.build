project('ufjfmltk', 'cpp',
  version : '1.0.0',
  default_options : ['warning_level=0', 'cpp_std=c++17'])

project_description = 'UFJF Machine Learning Toolkit'

core = subproject('ufjfmltk_core')
subproject('ufjfmltk_classifier')
subproject('ufjfmltk_valid')
subproject('ufjfmltk_ensemble')
subproject('ufjfmltk_featselect')
subproject('ufjfmltk_clusterer')
subproject('ufjfmltk_visual')


project_dependencies = [
  dependency('ufjfmltk_core', fallback : ['ufjfmltk_core', 'ufjfmltk_core_dep'], required:true),
  dependency('ufjfmltk_classifier', fallback : ['ufjfmltk_classifier', 'ufjfmltk_classifier_dep'], required:false),
  dependency('ufjfmltk_valid', fallback : ['ufjfmltk_valid', 'ufjfmltk_valid_dep'], required:false),
  dependency('ufjfmltk_ensemble', fallback : ['ufjfmltk_ensemble', 'ufjfmltk_ensemble_dep'], required:false),
  dependency('ufjfmltk_featselect', fallback : ['ufjfmltk_featselect', 'ufjfmltk_featselect_dep'], required:false),
  dependency('ufjfmltk_clusterer', fallback : ['ufjfmltk_clusterer', 'ufjfmltk_clusterer_dep'], required:false),
  dependency('ufjfmltk_visual', fallback : ['ufjfmltk_visual', 'ufjfmltk_visual_dep'], required:false)
]


project_target = shared_library(meson.project_name(), [],
  dependencies: project_dependencies,
  install : true,
  gnu_symbol_visibility : 'hidden',
  version: meson.project_version(),
  soversion: 0,
)

project_target_dep = declare_dependency(
  link_with : project_target
)

set_variable(meson.project_name() + '_dep', project_target_dep)

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : meson.project_name(),
  filebase : meson.project_name(),
  description : project_description,
  subdirs : 'ufjfmltk'
)