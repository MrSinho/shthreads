cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)


#||||       ||||
#||||PROJECT||||
#||||       ||||
project(shthreads C)



#||||       ||||
#||||LIBRARY||||
#||||       ||||
function(build_shthreads)
add_library(shthreads shthreads/src/shthreads.c)
target_include_directories(shthreads PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/shthreads/include
)
set_target_properties(shthreads PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
endfunction()



#||||       ||||
#||||SAMPLES||||
#||||       ||||
function(build_shthreads_sample)
add_executable(shthreads-sample sample/src/shthreads-sample.c)
target_link_libraries(shthreads-sample PUBLIC shthreads)
set_target_properties(shthreads-sample PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
endfunction()


#||||     ||||
#||||FLAGS||||
#||||     ||||
option(SH_THREADS_BUILD_SAMPLE CACHE OFF)



#||||    ||||
#||||MAIN||||
#||||    ||||
function(main)
build_shthreads()
if (SH_THREADS_BUILD_SAMPLE)
build_shthreads_sample()
endif(SH_THREADS_BUILD_SAMPLE)
endfunction()



#||||          ||||
#||||CMAKE MAIN||||
#||||          ||||
main()