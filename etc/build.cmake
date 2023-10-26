file(GLOB buildDirs "build*")
foreach(buildDir ${buildDirs})
	if(IS_DIRECTORY ${buildDir})
		message(STATUS "processing ${buildDir}")
		set(BUILD OFF)
		set(PARAM "")
		
		if(EXISTS "${buildDir}/build.ninja")
			set(BUILD ON)
			set(PARAM "all")
		endif()
		
		if(EXISTS "${buildDir}/Makefile")
			set(BUILD ON)
			set(PARAM "all")
		endif()
		
		if(NOT BUILD)
			message( STATUS "*** build system not recognized" )
		else()
			set(CMD "${CMAKE_COMMAND} --build ${buildDir} --target ${PARAM}")
			#message(STATUS "running '${CMD}'")
			execute_process(COMMAND           ${CMD} 
							#WORKING_DIRECTORY ${buildDir}
							COMMAND_ECHO      STDERR)
		endif()
			
		
	endif()
endforeach()
