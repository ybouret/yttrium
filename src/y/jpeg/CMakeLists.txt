project(jpeg C)

set(HERE ${CMAKE_CURRENT_SOURCE_DIR})
configure_file(jconfig.txt jconfig.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(LIBSOURCES
jaricom.c jcapimin.c jcapistd.c jcarith.c jccoefct.c jccolor.c 
        jcdctmgr.c jchuff.c jcinit.c jcmainct.c jcmarker.c jcmaster.c 
        jcomapi.c jcparam.c jcprepct.c jcsample.c jctrans.c jdapimin.c 
        jdapistd.c jdarith.c jdatadst.c jdatasrc.c jdcoefct.c jdcolor.c 
        jddctmgr.c jdhuff.c jdinput.c jdmainct.c jdmarker.c jdmaster.c 
        jdmerge.c jdpostct.c jdsample.c jdtrans.c jerror.c jfdctflt.c 
        jfdctfst.c jfdctint.c jidctflt.c jidctfst.c jidctint.c jquant1.c 
        jquant2.c jutils.c jmemmgr.c)
        

add_library(y-jpeg STATIC jmemnobs.c ${LIBSOURCES})

set(CSOURCES cjpeg.c rdppm.c rdgif.c rdtarga.c rdrle.c rdbmp.c rdswitch.c cdjpeg.c)
add_executable(y_cjpeg ${CSOURCES})
target_link_libraries(y_cjpeg y-jpeg)

set(DSOURCES djpeg.c wrppm.c wrgif.c wrtarga.c wrrle.c wrbmp.c rdcolmap.c cdjpeg.c)

add_executable(y_djpeg ${DSOURCES})
target_link_libraries(y_djpeg y-jpeg)

set(TRSOURCES  jpegtran.c rdswitch.c cdjpeg.c transupp.c)
add_executable(y_jpegtran ${DSOURCES})
target_link_libraries(y_jpegtran y-jpeg)

add_executable(y_rdjpgcom rdjpgcom.c)
target_link_libraries(y_rdjpgcom y-jpeg)

add_executable(y_wrjpgcom wrjpgcom.c)
target_link_libraries(y_wrjpgcom y-jpeg)

add_custom_target(y_test_jpeg
DEPENDS y_cjpeg y_djpeg
COMMAND ${CMAKE_COMMAND} -E ${cmake_rm} -f testout*
COMMAND y_djpeg -dct int -ppm -outfile testout.ppm testorig.jpg
COMMAND y_djpeg -dct int -gif -outfile testout.gif testorig.jpg
COMMAND y_djpeg -dct int -bmp -colors 256 -outfile testout.bmp testorig.jpg
COMMAND y_cjpeg -dct int -outfile testout.jpg testimg.ppm
COMMAND y_djpeg -dct int -ppm -outfile testoutp.ppm testprog.jpg
COMMAND y_cjpeg -dct int -progressive -opt -outfile testoutp.jpg testimg.ppm
COMMAND y_jpegtran -outfile testoutt.jpg testprog.jpg
WORKING_DIRECTORY ${HERE}
COMMENT "Testing JPEG"
VERBATIM)

#$(RM) testout*
#	./djpeg -dct int -ppm -outfile testout.ppm testorig.jpg
#	./djpeg -dct int -gif -outfile testout.gif testorig.jpg
#	./djpeg -dct int -bmp -colors 256 -outfile testout.bmp testorig.jpg
#	./cjpeg -dct int -outfile testout.jpg testimg.ppm
#	./djpeg -dct int -ppm -outfile testoutp.ppm testprog.jpg
#	./cjpeg -dct int -progressive -opt -outfile testoutp.jpg testimg.ppm
#	./jpegtran -outfile testoutt.jpg testprog.jpg
#	cmp testimg.ppm testout.ppm
#	cmp testimg.gif testout.gif
#	cmp testimg.bmp testout.bmp
#	cmp testimg.jpg testout.jpg
#	cmp testimg.ppm testoutp.ppm
#	cmp testimgp.jpg testoutp.jpg
#	cmp testorig.jpg testoutt.jpg


#Y_LinkMath(y_luac)
