# third_party library/code
add_subdirectory(lua)
add_subdirectory(zlib)
add_subdirectory(jpeg)

add_custom_target(ThirdParty 
DEPENDS y_lua z-example)
