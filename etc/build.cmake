file(GLOB buildDirs "build*")
foreach(buildDir ${buildDirs})
	#-------------------------------------------------------------------
	#
	# Loop over build directories
	#
	#-------------------------------------------------------------------
	if(IS_DIRECTORY ${buildDir})
		message(STATUS "processing ${buildDir}")
		set(BUILD OFF)	 
		
		# Ninja
		if(EXISTS "${buildDir}/build.ninja")
			set(BUILD ON)
			execute_process(COMMAND "${CMAKE_COMMAND}" "--build" "${buildDir}" "--target" "all"
							COMMAND_ECHO      STDERR)
		endif()
		
		# Makefile
		if(EXISTS "${buildDir}/Makefile")
			set(BUILD ON)
			execute_process(COMMAND "${CMAKE_COMMAND}" "--build" "${buildDir}" "--target" "all"
							COMMAND_ECHO      STDERR)
		endif()
		
		# Visual Studio
		if(EXISTS "${buildDir}/yttrium.sln")
			set(BUILD_ON)
			execute_process(COMMAND "${CMAKE_COMMAND}" "--build" "${buildDir}" "--target" "ALL_BUILD" "--config" "Release"
							COMMAND_ECHO      STDERR)
		endif()
		
		if(NOT BUILD)
			message( STATUS "*** build system not recognized" )
		endif()
			
		
	endif()
endforeach()
