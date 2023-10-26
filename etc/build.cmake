file(GLOB buildDirs "build*")
foreach(buildDir ${buildDirs})
	if(IS_DIRECTORY ${buildDir})
		message(STATUS "processing ${buildDir}")
		set(BUILD OFF)
		set(TGT  "all")
		
		if(EXISTS "${buildDir}/build.ninja")
			set(BUILD ON)
		endif()
		
		if(EXISTS "${buildDir}/Makefile")
			set(BUILD ON)
		endif()
		
		if(NOT BUILD)
			message( STATUS "*** build system not recognized" )
		else()
 			execute_process(COMMAND "${CMAKE_COMMAND}" "--build" "${buildDir}" "--target ${TGT}"
							#WORKING_DIRECTORY ${buildDir}
							COMMAND_ECHO      STDERR)
		endif()
			
		
	endif()
endforeach()
