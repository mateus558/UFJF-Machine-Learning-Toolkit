project('ufjfmltk', 'cpp',
  version : '0.40.0',
  default_options : ['warning_level=0', 'cpp_std=c++17','default_library=shared'])

modules_names = ['ufjfmltk-core', 'ufjfmltk-classifier', 'ufjfmltk-regressor', 'ufjfmltk-valid', 
                 'ufjfmltk-ensemble', 'ufjfmltk-featselect', 'ufjfmltk-clusterer', 'ufjfmltk-visual']

prefix = get_option('prefix')
project_install_dir = get_option('libdir')
build_type = get_option('buildtype')
library_type = get_option('default_library')

project_description = 'UFJF Machine Learning Toolkit'
project_subdir = 'ufjfmltk'
project_headers = ['ufjfmltk.hpp']
project_sources = []

message('Build type: ' + build_type)
message('Install directory: ' + prefix / project_install_dir)
message('Default library type: ' + library_type)
message('\nInstalling the necessary submodules, this can take some time. \n')

# if target_machine.system() == 'linux'
#   foreach module_name : modules_names
#     message('Installing '+ module_name +'...')
#     r = run_command('./module_installer.sh', module_name, prefix)
#     if r.returncode() != 0
#       message('Failed to install ' + module_name)
#     endif
#   endforeach
# endif

combined = []
project_dependencies = []
foreach module_name : modules_names
  module = subproject(module_name)
  module_name = '_'.join(module_name.split('-'))
  combined += module.get_variable(module_name+'_tgt').extract_all_objects()
  project_dependencies += module.get_variable(module_name+'_dep')
endforeach

subdir('docs')
install_headers(project_headers, subdir: project_subdir)

project_target = library(meson.project_name(), [project_headers, project_sources],
  dependencies: project_dependencies,
  objects: combined,
  install : true,
  install_dir : project_install_dir,
  version: meson.project_version(),
  soversion: '0'
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