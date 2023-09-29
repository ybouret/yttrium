########################################################################
##
## global settings
##
########################################################################
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS    ON)
set(CMAKE_SKIP_PREPROCESSED_SOURCE_RULES ON)
set(CMAKE_SKIP_ASSEMBLY_SOURCE_RULES     ON)

########################################################################
##
##
##  Find Configuration
##      
##              
########################################################################
string(COMPARE EQUAL "Debug"    "${CMAKE_BUILD_TYPE}" Y_DEBUG)
string(COMPARE EQUAL "Release"  "${CMAKE_BUILD_TYPE}" Y_RELEASE)
        
if(Y_DEBUG OR Y_RELEASE)
        message( STATUS "[yttrium] CMAKE_BUILD_TYPE is set to '${CMAKE_BUILD_TYPE}'")
else()  
        message( STATUS "[yttrium] CMAKE_BUILD_TYPE fallback to 'Debug'" )
        set(CMAKE_BUILD_TYPE "Debug")
        set(YACK_DEBUG ON)
endif()

# macro to modify flags
macro(Y_ADD_FLAGS flag)
    message( STATUS "[yttrium] adding C/C++ FLAGS : '${flag}'" )
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${flag}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
endmacro()

########################################################################
##
##
##  Find Compiler Family
##
##
########################################################################
set(Y_KNOWN_COMPILER OFF)
set(Y_COMPILERS      "")
get_filename_component(Y_CC ${CMAKE_C_COMPILER} NAME_WE)

message( STATUS "[yttrium] cmake config     : '${CMAKE_BUILD_TYPE}'" )
message( STATUS "[yttrium] system name      : '${CMAKE_SYSTEM_NAME}'" )
message( STATUS "[yttrium] found C compiler : '${Y_CC}'" )
message( STATUS "[yttrium] system processor : '${CMAKE_HOST_SYSTEM_PROCESSOR}'" )
message( STATUS "[yttrium] sizeof(void*)    : '${CMAKE_SIZEOF_VOID_P}'" )

set(Y_BITS "")

if( "${CMAKE_SIZEOF_VOID_P}" STREQUAL "4" )
	set(Y_BITS "32")
endif()

if( "${CMAKE_SIZEOF_VOID_P}" STREQUAL "8" )
	set(Y_BITS "64")
endif()

if("" STREQUAL "${Y_BITS}")
	message(FATAL_ERROR "couldn't guess BITS")
endif()

message( STATUS "[yttrium] native bits      : '${Y_BITS}'" )

string(COMPARE EQUAL "Darwin"  ${CMAKE_SYSTEM_NAME} Y_DARWIN)
string(COMPARE EQUAL "FreeBSD" ${CMAKE_SYSTEM_NAME} Y_FREEBSD)
string(COMPARE EQUAL "Linux"   ${CMAKE_SYSTEM_NAME} Y_LINUX)
string(COMPARE EQUAL "SunOS"   ${CMAKE_SYSTEM_NAME} Y_SUNOS)
string(COMPARE EQUAL "OpenBSD" ${CMAKE_SYSTEM_NAME} Y_OPENBSD)
string(COMPARE EQUAL "Windows" ${CMAKE_SYSTEM_NAME} Y_WINDOWS)

set(cmake_rm "")

if("${CMAKE_VERSION}" VERSION_LESS "3.17" )
        # old cmake -E
         set(cmake_rm "remove")
else()
         set(cmake_rm "rm")
endif()
message( STATUS "[yttrium] removing with    : '${cmake_rm}'" )


########################################################################
##
##
##  Find Compiler Version, variables and macro
##
##
########################################################################
set(Y_COMPILER_VERSION "")
set(Y_COMPILER_MAJOR   "")
set(Y_COMPILER_MINOR   "")
set(Y_COMPILER_PATCH   "")

########################################################################
##
##
## macro will be call for gnu/clang/intel...
##
##
########################################################################
macro(Y_FIND_COMPILER_VERSION)
        # call cc --version
        execute_process(COMMAND ${CMAKE_C_COMPILER} --version
                                        OUTPUT_VARIABLE Y_RAW_COMPILER_VERSION
                                        RESULT_VARIABLE Y_FIND_COMPILER_VERSION_SUCCESS
                                        ERROR_QUIET
                                        )
        if( NOT ("0" STREQUAL "${Y_FIND_COMPILER_VERSION_SUCCESS}") )
                message( FATAL_ERROR "[yttrium] Couldn't find compiler version")
        endif()
        # get version pattern
        string( REGEX MATCH    "[0-9]+\\.[0-9]+\\.[0-9]+"         YC_VERSION "${Y_RAW_COMPILER_VERSION}" )
        string( REGEX REPLACE  "([0-9]+)\\.[0-9]+\\.[0-9]+" "\\1" YC_MAJOR   "${YC_VERSION}" )
        string( REGEX REPLACE  "[0-9]+\\.([0-9]+)\\.[0-9]+" "\\1" YC_MINOR   "${YC_VERSION}" )
        string( REGEX REPLACE  "[0-9]+\\.[0-9]+\\.([0-9]+)" "\\1" YC_PATCH   "${YC_VERSION}" )
        message( STATUS "[yttrium] '${Y_CC}' is <${YC_VERSION}>: major=${YC_MAJOR}, minor=${YC_MINOR}, patchlevel=${YC_PATCH}" )
        set(Y_COMPILER_VERSION ${YC_VERSION}  )
        set(Y_COMPILER_MAJOR   ${YC_MAJOR}    )
        set(Y_COMPILER_MINOR   ${YC_MINOR}    )
        set(Y_COMPILER_PATCH   ${YC_PATCH}    )
endmacro()

########################################################################
##
##
##  Clang
##
##
########################################################################
if(${Y_CC} MATCHES "clang.*" )

        set(Y_KNOWN_COMPILER ON)
        set(Y_COMPILERS      "clang")
        set(Y_CLANG          ON)
        Y_FIND_COMPILER_VERSION()

        set(CMAKE_C_FLAGS   "-Wall -Wextra -pipe -fPIC")
        set(CMAKE_CXX_FLAGS "-Wall -Wextra -pipe -fPIC -fexceptions -Weffc++ -std=c++11" )

        set(Y_FLAGS_DEBUG   "-O0 -g")
        set(Y_FLAGS_RELEASE "-O2 -DNDEBUG=1 -D_FORTIFY_SOURCE=2")
endif()


########################################################################
##
##
##  GNU
##
##
########################################################################
if(${Y_CC} MATCHES "gcc.*" )

        set(Y_KNOWN_COMPILER ON)
        set(Y_COMPILERS      "gnu")
        SET(Y_GNU            ON)
        Y_FIND_COMPILER_VERSION()

        set(CMAKE_C_FLAGS   "-Wall -pipe -fPIC -std=gnu99")
        set(CMAKE_CXX_FLAGS "-Wall -pipe -fPIC -fexceptions -Weffc++" )

        if(${Y_COMPILER_MAJOR} GREATER 4)
                Y_ADD_FLAGS("-Wextra")
        endif()

        if(${Y_COMPILER_VERSION} VERSION_GREATER_EQUAL "4.8.1")
                 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        endif()

        set(Y_FLAGS_DEBUG   "-O0 -g")
        set(Y_FLAGS_RELEASE "-O2 -DNDEBUG=1 -D_FORTIFY_SOURCE=2")


endif()


########################################################################
##
##
##  Intel
##
##
########################################################################
if(${Y_CC} MATCHES "icc.*" )

	set(Y_KNOWN_COMPILER ON)
	set(Y_COMPILERS      "intel")
	set(Y_INTEL          ON)
	Y_FIND_COMPILER_VERSION()

	set(CMAKE_C_FLAGS   "-Wall -pipe -wd981 -fPIC -xHost")
	set(CMAKE_CXX_FLAGS "-Wall -pipe -wd981 -fPIC -xHost -fexceptions -std=c++11" )
	
	set(Y_FLAGS_DEBUG   "-O0 -g")
	set(Y_FLAGS_RELEASE "-O2 -DNDEBUG=1 -D_FORTIFY_SOURCE=2")
	
endif()

########################################################################
##
##
##  Microsoft
##
##
########################################################################
if(${Y_CC} STREQUAL "cl" )

	set(Y_KNOWN_COMPILER ON)
	set(Y_COMPILERS      "microsoft")
	set(Y_MICROSOFT      ON)
	
	set(CMAKE_C_FLAGS   "-nologo")
	set(CMAKE_CXX_FLAGS "-nologo -EHsc" )
	set(Y_FLAGS_DEBUG   "")
	set(Y_FLAGS_RELEASE "-Ox -DNDEBUG=1")
	 
endif()

########################################################################
##
##
##  checking known compilers
##
##
########################################################################
if(NOT Y_KNOWN_COMPILER)
    message( FATAL_ERROR "[yttrium] unknown '${Y_CC}'")
endif()

########################################################################
##
##
##  updating flags
##
##
########################################################################
set(CMAKE_C_FLAGS_RELEASE   ${Y_FLAGS_RELEASE})
set(CMAKE_CXX_FLAGS_RELEASE ${Y_FLAGS_RELEASE})
set(CMAKE_C_FLAGS_DEBUG     ${Y_FLAGS_DEBUG})
set(CMAKE_CXX_FLAGS_DEBUG   ${Y_FLAGS_DEBUG})

########################################################################
##
##
## creating UUID 
##
##
########################################################################
set(Y_UUID "-${Y_COMPILERS}${Y_BITS}")
if(Y_DEBUG)
	set(Y_UUID "${Y_UUID}d")
endif()
message( STATUS "[yttrium] UUID='${Y_UUID}'" )

########################################################################
##
##
##  Linking
##
##
########################################################################
macro(Y_LinkLibraries target)
    set(libs y)
    if(Y_LINUX OR Y_FREEBSD)
        list(APPEND libs pthread)
    endif()
    
    if(Y_LINUX)
		list(APPEND libs rt dl)
    endif()
		
	if(Y_WINDOWS)
		if(Y_GNU)
			target_link_libraries(${target} -static-libgcc -static-libstdc++) 
		endif()
	endif()
    
    target_link_libraries(${target} ${ARGN} ${libs} )
endmacro()

########################################################
#
# libm necessary or not
#
########################################################
set(Y_NeedsMath OFF)

if(Y_SUNOS OR Y_LINUX OR Y_FREEBSD)
	set(Y_NeedsMath ON)
endif()

if(Y_NeedsMath)
	macro(Y_LinkMath target)
		message( STATUS "[yttrium] --> ${target} requires libm (EXPLICIT)" )
		target_link_libraries(${target} m)
	endmacro()
else()
	macro(Y_LinkMath target)
		message( STATUS "[yttrium] --> ${target} requires libm (IMPLICIT)" )
	endmacro()
endif()



