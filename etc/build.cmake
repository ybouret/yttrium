
include(ProcessorCount)
ProcessorCount(N)
if(N EQUAL 0)
	set(N 1)
endif()

message( STATUS "#processors=${N}")

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
			execute_process(COMMAND "${CMAKE_COMMAND}" "--build" "${buildDir}" "--target" "all" "--parallel" ${N}
							COMMAND_ECHO      STDERR)
		endif()
		
		# Makefile
		if(EXISTS "${buildDir}/Makefile")
			set(BUILD ON)
			execute_process(COMMAND "${CMAKE_COMMAND}" "--build" "${buildDir}" "--target" "all" "--parallel" ${N}
							COMMAND_ECHO      STDERR)
		endif()
		
		# Visual Studio
		if(EXISTS "${buildDir}/yttrium.sln")
			set(BUILD ON)
			execute_process(COMMAND "${CMAKE_COMMAND}" "--build" "${buildDir}" "--target" "ALL_BUILD" "--config" "Release" #"--parallel" ${N}
							COMMAND_ECHO      STDERR)
		endif()
		
		if(NOT BUILD)
			message( STATUS "*** build system not recognized in ${buildDir}" )
		endif()
			
		
	endif()
endforeach()
