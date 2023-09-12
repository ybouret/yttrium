# third_party library/code

message( STATUS "CMAKE_C_FLAGS      ,  = ${CMAKE_C_FLAGS}")
string(REPLACE "-Wall" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
string(STRIP ${CMAKE_C_FLAGS} CMAKE_C_FLAGS)
string(REPLACE "-Wextra" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
string(STRIP ${CMAKE_C_FLAGS} CMAKE_C_FLAGS)
message( STATUS "CMAKE_C_FLAGS         = ${CMAKE_C_FLAGS}")
message( STATUS "CMAKE_C_FLAGS_DEBUG   = ${CMAKE_C_FLAGS_DEBUG}")
message( STATUS "CMAKE_C_FLAGS_RELEASE = ${CMAKE_C_FLAGS_RELEASE}")


add_subdirectory(lua)
add_subdirectory(zlib)
add_subdirectory(jpeg)
add_subdirectory(bzip2)
add_subdirectory(png)
add_subdirectory(jbig)

add_custom_target(ThirdParty 
DEPENDS 
y_lua 
z-example 
y_test_jpeg 
y_test_bzip2
y_test_jbig
y_test_jbig85)
