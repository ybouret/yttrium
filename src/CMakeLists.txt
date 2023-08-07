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
    
    add_custom_target(utest-${id}
		COMMAND ${CMAKE_CTEST_COMMAND} -C ${CMAKE_BUILD_TYPE}
		COMMENT "[yttrium] Unit Tests on <core>"
		VERBATIM)
endmacro()

## macro to create a library from a series of subdir
## assuming srcs is existing
macro(Y_CreateLibrary id)
    message( STATUS "[yttrium] <${id}>")
    #set(srcs "")
    foreach(subdir ${ARGN})
        message( STATUS "        |- ${subdir}")
        file(GLOB decl y/${subdir}/*.h   y/${subdir}/*.hpp)
        file(GLOB impl y/${subdir}/*.c   y/${subdir}/*.cpp y/${subdir}/*.hxx)
        list(APPEND srcs ${decl} ${impl})
        source_group(${subdir} FILES ${decl} ${impl})
    endforeach()
    add_library(${id} STATIC ${srcs})
    message( STATUS "[yttrium] <${id}/>")
endmacro()

## macro to load unit tests into regression
set(Y_Regressions "")
macro(Y_Regression id)
	list(APPEND Y_Regressions "utest-${id}")
	message( STATUS "Regressions: ${Y_Regressions}")
endmacro()

# all subdirectory
add_subdirectory(core)
add_subdirectory(jive)

message( STATUS "[yttrium] Regressions: ${Y_Regressions}")


# looking for Doxygen to generate documentation
find_program(doxygen doxygen)
if("${doxygen}" EQUAL "doxygen-NOTFOUND" )
    message( STATUS "missing Doxygen")
else()
    message( STATUS "found Doxygen at ${doxygen}")
    add_custom_target(dox ${doxygen}
                        WORKING_DIRECTORY ${HERE}/..)
endif()
