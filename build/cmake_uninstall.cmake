if(NOT EXISTS "/home/mateuscmarim/Documents/UFJF-Machine-Learning-Toolkit/build/install_manifest.txt")
    message(WARNING "Cannot find install manifest: \"/home/mateuscmarim/Documents/UFJF-Machine-Learning-Toolkit/build/install_manifest.txt\"")
    return()
endif()

file(READ "/home/mateuscmarim/Documents/UFJF-Machine-Learning-Toolkit/build/install_manifest.txt" files)
string(STRIP "${files}" files)
string(REGEX REPLACE "\n" ";" files "${files}")
list(REVERSE files)
foreach(file ${files})
    message(STATUS "Uninstalling: $ENV{DESTDIR}${file}")
    if(EXISTS "$ENV{DESTDIR}${file}")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E remove "$ENV{DESTDIR}${file}"
            OUTPUT_VARIABLE rm_out
            RESULT_VARIABLE rm_retval)
        if(NOT "${rm_retval}" EQUAL 0)
            message(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
        endif()
    else()
        message(STATUS "File \"$ENV{DESTDIR}${file}\" does not exist.")
    endif()
endforeach(file)
