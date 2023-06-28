/**!  \file */

#ifndef Y_Config_Platform_Included
#define Y_Config_Platform_Included 1

#if defined(Y_PLATFORM)
#error "Y_PLATFORM shouldn't be defined here!!"
#endif

#if defined(__APPLE__)
#define Y_Darwin 1
#define Y_BSD    1
#define Y_PLATFORM "Darwin"
#endif

#if defined(__FreeBSD__)
#define Y_FreeBSD  1
#define Y_BSD      1
#define Y_PLATFORM "FreeBSD"
#endif

#if defined(__linux__)
#define Y_Linux 1
#define Y_BSD   1
#define Y_PLATFORM "Linux"
#endif

#if defined(__OpenBSD__)
#define Y_OpenBSD   1
#define Y_BSD       1
#define Y_PLATFORM "OpenBSD"
#endif

#if defined(__sun__)
#define Y_SunOS    1
#define Y_BSD      1
#define Y_PLATFORM "SunOS"
#endif

#if defined(_WIN32)
#define Y_WIN     1
#define Y_PLATFORM "Windows"
#endif

#if !defined(Y_PLATFORM)
#error "Y_PLATFORM was not found"
#endif



#endif

