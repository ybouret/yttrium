project(yttrium)

## global settings
cmake_minimum_required(VERSION 3.0)
include(../etc/y.cmake)

set(HERE ${CMAKE_CURRENT_SOURCE_DIR})
message( STATUS "HERE='${HERE}'")

set(CMAKE_INSTALL_PREFIX "${HERE}/../sdk")

## macro to create a series of tests
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

## macro to create a library from a series of subdir
## assuming srcs is existing
macro(Y_CreateLibrary id)
    message( STATUS "[yttrium] <${id}>")
    #set(srcs "")
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

macro(Y_LinkLibraries target)
    set(libs y)
    if(Y_LINUX)
        list(APPEND libs pthread)
    endif()
    
    if(Y_LINUX)
		list(APPEND libs rt)
    endif()
    
    target_link_libraries(${target} ${libs})
endmacro()

# all subdirectory
add_subdirectory(core)

# looking for Doxygen to generate documentation
find_program(doxygen doxygen)
if("${doxygen}" EQUAL "doxygen-NOTFOUND" )
    message( STATUS "missing Doxygen")
else()
    message( STATUS "found Doxygen at ${doxygen}")
    add_custom_target(dox ${doxygen}
                        WORKING_DIRECTORY ${HERE}/..)
endif()
