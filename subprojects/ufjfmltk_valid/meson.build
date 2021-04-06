project('ufjfmltk_valid', 'cpp',
  version : '1.0.0',
  default_options : ['warning_level=0', 'cpp_std=c++17'])
project_description = 'UFJF-MLTK validation library.'

project_headers = [
  'include/Validation.hpp'
]

public_headers = include_directories('include')

project_target = static_library(
  meson.project_name(),
  [],
  install : true,
  gnu_symbol_visibility : 'hidden',
  include_directories : public_headers,
)

# =======
# Project
# =======

# Make this library usable as a Meson subproject.
project_dep = declare_dependency(
  include_directories: public_headers
)
set_variable(meson.project_name() + '_dep', project_dep)

# Make this library usable from the system's
# package manager.
install_headers(project_headers, subdir : 'ufjfmltk')

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : meson.project_name(),
  filebase : meson.project_name(),
  description : project_description,
  subdirs : 'ufjfmltk'
)