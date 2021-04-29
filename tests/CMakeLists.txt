message("Testing enabled.")
enable_testing()
include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/refs/tags/release-1.10.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

if(CODE_COVERAGE)
    include(CodeCoverage)
    append_coverage_compiler_flags()
endif(CODE_COVERAGE)

set(CORE_TESTS core_test)
add_subdirectory(core)
set(CLASSIFIER_TESTS classifier_test)
add_subdirectory(classifier)

set(COV_TARGETS ${CLASSIFIER_TESTS} ${CORE_TESTS})
message("TARGETS: ${COV_TARGETS}")
if(CODE_COVERAGE)
    setup_target_for_coverage_gcovr_html(
            NAME coverage
            EXECUTABLE ctest
            DEPENDENCIES ${COV_TARGETS} ufjfmltk-core ufjfmltk-classifier ufjfmltk-valid
            EXCLUDE "cmake*;subprojects/ufjfmltk-classifier/include/ufjfmltk/classifier/hnswlib/*"
    )

endif(CODE_COVERAGE)