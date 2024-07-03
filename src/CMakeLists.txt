cmake_minimum_required(VERSION 3.5)
project(yttrium)

################################################################################
##
## global settings
##
################################################################################
include(../etc/y.cmake)

set(HERE ${CMAKE_CURRENT_SOURCE_DIR})
message( STATUS "HERE='${HERE}'")

set(CMAKE_INSTALL_PREFIX "${HERE}/../sdk")
INSTALL(FILES 
../etc/y.cmake 
../etc/fltk.cmake
DESTINATION etc)

################################################################################
##
## macro to create a series of tests
##
################################################################################
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
		DEPENDS test-${id}
		COMMENT "[yttrium] Unit Tests on <${id}>"
		VERBATIM)
endmacro()

################################################################################
##
## macro to create a library from a series of subdir
## assuming srcs is existing
##
################################################################################
macro(Y_CreateLibrary id)
    message( STATUS "[yttrium] <${id}>")
    #set(srcs "")
    foreach(subdir ${ARGN})
        message( STATUS "        |- ${subdir}")
        file(GLOB decl y/${subdir}/*.h   y/${subdir}/*.hpp)
        file(GLOB impl y/${subdir}/*.c   y/${subdir}/*.cpp y/${subdir}/*.hxx)
        list(APPEND srcs ${decl} ${impl})
        source_group(${subdir} FILES ${decl} ${impl})
        install(FILES ${decl} DESTINATION include/y/${subdir})
    endforeach()
    add_library(${id} STATIC ${srcs})
    install(TARGETS ${id} ARCHIVE DESTINATION lib) 
    message( STATUS "[yttrium] <${id}/>")
endmacro()

################################################################################
##
## macro to load unit tests into regression
##
################################################################################
set(Y_Regressions "")
macro(Y_Regression id)
	list(APPEND Y_Regressions "utest-${id}")
    set(Y_Regressions ${Y_Regressions} PARENT_SCOPE)
	#message( STATUS "Regressions: ${Y_Regressions}")
endmacro()

################################################################################
##
## all subdirectories
##
################################################################################
add_subdirectory(core)
add_subdirectory(jive)
add_subdirectory(json)
add_subdirectory(mkl)
add_subdirectory(lua++)
#add_subdirectory(chem)
add_subdirectory(chemical)
add_subdirectory(ink)
add_subdirectory(net)
add_subdirectory(mpi++)
add_subdirectory(fltk)

################################################################################
##
## making the regression target
##
################################################################################
message( STATUS "[yttrium] Regressions: ${Y_Regressions}")
add_custom_target(Regression DEPENDS ${Y_Regressions})

################################################################################
##
## third party
##
################################################################################
add_subdirectory(y)

################################################################################
##
## looking for Doxygen to generate documentation
##
################################################################################
find_program(doxygen doxygen)
if("${doxygen}" STREQUAL "doxygen-NOTFOUND" )
    message( STATUS "missing Doxygen")
else()
    message( STATUS "found Doxygen at ${doxygen}")
    add_custom_target(dox ${doxygen}
                        WORKING_DIRECTORY ${HERE}/..)
endif()

add_subdirectory(cli)

