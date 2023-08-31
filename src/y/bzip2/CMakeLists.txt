project(bzip2 C)

set(HERE ${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-D_FILE_OFFSET_BITS=64)

add_library(y-bz2 STATIC 
blocksort.c
huffman.c  
crctable.c   
randtable.c  
compress.c  
decompress.c 
bzlib.c
)

add_executable(y_bzip2 bzip2.c)
target_link_libraries(y_bzip2 y-bz2)

add_custom_target(y_test_bzip2
DEPENDS y_bzip2
COMMAND y_bzip2  -1  < sample1.ref > sample1.rb2
COMMAND y_bzip2  -2  < sample2.ref > sample2.rb2
COMMAND y_bzip2  -3  < sample3.ref > sample3.rb2
COMMAND y_bzip2  -d  < sample1.bz2 > sample1.tst
COMMAND y_bzip2  -d  < sample2.bz2 > sample2.tst
COMMAND y_bzip2  -ds < sample3.bz2 > sample3.tst
COMMAND ${CMAKE_COMMAND} -E compare_files sample1.bz2 sample1.rb2 
COMMAND ${CMAKE_COMMAND} -E compare_files sample2.bz2 sample2.rb2
COMMAND ${CMAKE_COMMAND} -E compare_files sample3.bz2 sample3.rb2
COMMAND ${CMAKE_COMMAND} -E compare_files sample1.tst sample1.ref
COMMAND ${CMAKE_COMMAND} -E compare_files sample2.tst sample2.ref
COMMAND ${CMAKE_COMMAND} -E compare_files sample3.tst sample3.ref
#COMMAND ${CMAKE_COMMAND} -E ${cmake_rm} -f 'sample*.rb2'
WORKING_DIRECTORY ${HERE}
COMMENT           "Testing BZIP2"
VERBATIM)