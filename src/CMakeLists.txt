project(yttrium)
cmake_minimum_required(VERSION 3.0)
include(../etc/y.cmake)

set(HERE ${CMAKE_CURRENT_SOURCE_DIR})
message( STATUS "HERE='${HERE}'")

macro(Y_CreateTest id)
    message( STATUS "[yttrium] <test-${id}>")
    file(GLOB srcs test-*.cpp)
    foreach(subdir ${ARGN})
        message( STATUS "        |- ${subdir}")
        file(GLOB extra ${subdir}/*.cpp)
        source_group(${subdir} FILES ${extra})
        list(APPEND srcs ${extra})
    endforeach()
    
    add_executable(test-${id} ${srcs} main.cpp)
    message( STATUS "[yttrium] <test-${id}/>")
endmacro()


macro(Y_CreateLibrary id)
    message( STATUS "[yttrium] <${id}>")
    set(srcs "")
    foreach(subdir ${ARGN})
        message( STATUS "        |- ${subdir}")
        file(GLOB decl y/${subdir}/*.h   y/${subdir}/*.hpp)
        file(GLOB impl y/${subdir}/*.c   y/${subdir}/*.cpp)
        list(APPEND srcs ${decl} ${impl})
        source_group(${subdir} FILES ${decl} ${impl})
    endforeach()
    add_library(${id} STATIC ${srcs})
    message( STATUS "[yttrium] <${id}/>")
endmacro()

add_subdirectory(core)


find_program(doxygen doxygen)
if("${doxygen}" EQUAL "doxygen-NOTFOUND" )
    message( STATUS "missing Doxygen")
else()
    message( STATUS "found Doxygen at ${doxygen}")
    add_custom_target(dox ${doxygen}
                        WORKING_DIRECTORY ${HERE}/..)
endif()
