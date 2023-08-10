project(lua)

set(HERE ${CMAKE_CURRENT_SOURCE_DIR})
set(DEMO ${HERE}/demo)
MESSAGE( STATUS "[yttrium] Lua in ${HERE}" )

set(srcs lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c lmem.c lobject.c lopcodes.c lparser.c lstate.c lstring.c ltable.c ltm.c lundump.c lvm.c lzio.c lauxlib.c lbaselib.c lcorolib.c ldblib.c liolib.c lmathlib.c loadlib.c loslib.c lstrlib.c ltablib.c lutf8lib.c linit.c)

MESSAGE( STATUS "[yttrium] library    : y-lua" )
add_library(y-lua STATIC ${srcs})

MESSAGE( STATUS "[yttrium] interpreter: y_lua" )
add_executable(y_lua lua.c)
target_link_libraries(y_lua y-lua)

add_custom_command(TARGET y_lua POST_BUILD
COMMAND y_lua ${DEMO}/hello.lua
COMMAND y_lua ${DEMO}/globals.lua
COMMAND y_lua ${DEMO}/bisect.lua
COMMAND y_lua ${DEMO}/sieve.lua
COMMAND y_lua ${DEMO}/account.lua
COMMENT "[yttrium] Testing Lua"
VERBATIM)

MESSAGE( STATUS "[yttrium] compiler   : y_luac" )
add_executable(y_luac luac.c)
target_link_libraries(y_luac y-lua)


