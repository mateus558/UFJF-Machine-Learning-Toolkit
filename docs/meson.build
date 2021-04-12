target_name = 'docs'
cdata = configuration_data()
cdata.set('TOP_SRCDIR', meson.source_root())
cdata.set('TOP_BUILDDIR', meson.build_root())
cdata.set('OUTPUT_DIR', join_paths(meson.build_root(), target_name))
cdata.set('README_PATH', join_paths(meson.source_root(),'README.md'))
cdata.set('VERSION', meson.project_version())
cdata.set('PROJECT_NAME', 'UFJF - Machine Learning Toolkit')
cdata.set('PROJECT_LOGO', 'logo.png')

doxyfile = configure_file(input: 'Doxyfile.in',
                          output: 'Doxyfile',
                          configuration: cdata,
                          install: false)

doxygen = find_program('doxygen', required:false)
if doxygen.found()
    doc_target = run_target(target_name, command: [doxygen, doxyfile])
endif