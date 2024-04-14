################################################################################
##
## Simplified FLTK configuration, based on fltk-config
##
################################################################################
message( STATUS "<FTLK>" )

################################################################################
##
## fltk-config and fluid
##
################################################################################
set(FLTK_FOUND   OFF)
set(FLTK_VERBOSE ON)

if(MSVC)

else(MSVC)

	set(FLTK_BIN $ENV{FLTK_BIN} CACHE STRING "Override fltk-config location" )
	if( "" STREQUAL "${FLTK_BIN}" )
		# automatic detection
		message( STATUS "Looking for default FLTK")
		find_program( FLTK-CONFIG fltk-config )
		if( ${FLTK-CONFIG} STREQUAL FLTK-CONFIG-NOTFOUND )
			message( STATUS "Can't find fltk-config" )
		else()
			# got fltk config
			find_program( FLUID fluid )
			if( ${FLUID} STREQUAL FLUID-NOTFOUND )
				message( STATUS "Can't find fluid!" )
			else()
				# got fluid
				set(FLTK_FOUND ON)
			endif()
		endif()
		
	else()
		# manual override
		set(FLTK-CONFIG "${FLTK_BIN}/fltk-config" )
		set(FLUID       "${FLTK_BIN}/fluid"       )
		set(FLTK_FOUND ON)
	endif()

if(FLTK_FOUND)

	message( STATUS "@FLTK fltk-config='${FLTK-CONFIG}'" )
	message( STATUS "@FLTK fluid='${FLUID}" )

	############################################################################
	##
	## Compilation
	##
	############################################################################

	#-----------------------------------------------------------------------
	# get cxxflags 
	#-----------------------------------------------------------------------
	message( STATUS "  @FLTK query cxxflags..." )
	find_program(BASH bash REQUIRED)
	if(FLTK_VERBOSE)
		message( STATUS "@FLTK bash='${BASH}'" )
	endif()
	#exec_program(${BASH} ARGS "${FLTK-CONFIG}" --cxxflags OUTPUT_VARIABLE FLTK-CXXFLAGS)
	execute_process(COMMAND ${BASH} "${FLTK-CONFIG}" --cxxflags 
	                OUTPUT_VARIABLE FLTK-CXXFLAGS
					OUTPUT_STRIP_TRAILING_WHITESPACE)
	if(FLTK_VERBOSE)
		message( STATUS "@FLTK-CXXFLAGS='${FLTK-CXXFLAGS}'" )
	endif()

	#-----------------------------------------------------------------------
	# extract include directories
	#-----------------------------------------------------------------------
	string( REGEX MATCHALL "[-][I]([^ ;])+" FLTK-INCLUDES ${FLTK-CXXFLAGS} )
	string( REPLACE  "-I" "" FLTK-INCLUDES "${FLTK-INCLUDES}")
	list(REMOVE_DUPLICATES FLTK-INCLUDES)
	if(FLTK_VERBOSE)
		message( STATUS "@FLTK-INCLUDES='${FLTK-INCLUDES}'" )
	endif()
	include_directories(${FLTK-INCLUDES})

	#-----------------------------------------------------------------------
	# extract definitions
	#-----------------------------------------------------------------------
	string( REGEX MATCHALL "[-][D]([^ ;])+" FLTK-DEFINES ${FLTK-CXXFLAGS} )
	if(FLTK_VERBOSE)
		message( STATUS "@FLTK-DEFINES='${FLTK-DEFINES}'")
	endif()
	add_definitions(${FLTK-DEFINES})

	#-----------------------------------------------------------------------
	# extract FLTK link directory
	#-----------------------------------------------------------------------
	#EXEC_PROGRAM( bash ARGS ${FLTK-CONFIG} --ldflags OUTPUT_VARIABLE FLTK-LDFLAGS)
	#message( STATUS "  @FLTK-LDFLAGS='${FLTK-LDFLAGS}'")
	#STRING( REGEX MATCHALL "[-][L]([^ ;])+" FLTK-LINK-DIR ${FLTK-LDFLAGS} )
	#STRING( REPLACE  "-L" "" FLTK-LINK-DIR "${FLTK-LINK-DIR}")
	#message( STATUS "  @FLTK-LINK-DIR='${FLTK-LINK-DIR}'" )
	#LINK_DIRECTORIES( ${FLTK-LINK-DIR} )

	########################################################################
	## Linking
	########################################################################
	## Link with optional FLTK libraries
	macro(TARGET_LINK_FLTK THE_TARGET)

		#initialize arguments for fltk-config
		set(_fltk_ldflags "--ldflags" )

		#collect extra arguments (images,gl,...)
		foreach( extra ${ARGN} )
			set(_fltk_ldflags "${_fltk_ldflags} --use-${extra}" )
		endforeach( extra ${ARGN} )

		#get all args
		#message( STATUS "@FLTK --> ${THE_TARGET} | 'fltk-config ${_fltk_ldflags}'" )
		#exec_program( ${BASH} ARGS ${FLTK-CONFIG} ${_fltk_ldflags} OUTPUT_VARIABLE FLTK-LDFLAGS)
		execute_process( COMMAND ${BASH} ${FLTK-CONFIG} ${_fltk_ldflags} OUTPUT_VARIABLE FLTK-LDFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
		if(FLTK_VERBOSE)
			message( STATUS "@FLTK-LDFLAGS='${FLTK-LDFLAGS}'")
		endif()
 
		#extract libraries, link directories is already set
		#STRING( REGEX MATCHALL "([-][l]([^ ;])+)|(-framework ([^ ;])+)" FLTK-LIBS ${FLTK-LDFLAGS})
		#STRING( REPLACE  "-l" "" FLTK-LIBS "${FLTK-LIBS}")
		#message( STATUS "${THE_TARGET} needs '${FLTK-LIBS}'")
		
		#and declare libraries to be linked !
		#TARGET_LINK_LIBRARIES(${THE_TARGET} ${FLTK-LIBS})
		TARGET_LINK_LIBRARIES(${THE_TARGET} ${FLTK-LDFLAGS})
	endmacro(TARGET_LINK_FLTK)

	########################################################################
	## fluid compiler API
	## FLUID_UIC(UI [PATH]) will produce UI_SOURCE UI_HEADER UI_SOURCES
	########################################################################
	macro(FLUID_UIC THE_UI)
		set(_UI_FL     "${CMAKE_CURRENT_SOURCE_DIR}/${THE_UI}.fl" )
		set(_UI_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/${THE_UI}.cxx")
		set(_UI_HEADER "${CMAKE_CURRENT_BINARY_DIR}/${THE_UI}.h")
		message( STATUS "**** <${THE_UI}> will compile"   )
		message( STATUS "** fluid : ${_UI_FL}"     )
		message( STATUS "** source: ${_UI_SOURCE}" )
		message( STATUS "** header: ${_UI_HEADER}" )
		message( STATUS "@FLTK: GUI from <${THE_UI}.fl>" )
		
		#create the command
		add_custom_command( OUTPUT  ${_UI_SOURCE} ${_UI_HEADER}
							COMMAND ${FLUID} -c -o ${_UI_SOURCE} -h ${_UI_HEADER} ${_UI_FL} 
							DEPENDS ${_UI_FL}
							COMMENT "Generating Code for ${THE_UI}" )
		
		#register the output directory	
		include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

		#build the global variables
		set( ${THE_UI}_SOURCE  ${_UI_SOURCE} )
		set( ${THE_UI}_HEADER  ${_UI_HEADER} )
		set( ${THE_UI}_SOURCES ${_UI_SOURCE} ${_UI_HEADER} ${_UI_FL} )
	endmacro(FLUID_UIC)

endif(FLTK_FOUND)

macro(Y_FLTK_SILENCE)
	message( STATUS "@FLTK: Removing warnings" )
	foreach( word IN ITEMS "-Weffc++" "-Wextra" )
		string(REPLACE "${word}" "" CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}"   )
		string(REPLACE "${word}" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" )
	endforeach()
endmacro(Y_FLTK_SILENCE)

endif(MSVC)
