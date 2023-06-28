/*! \file */

#ifndef Y_Config_Compiler_Included
#define Y_Config_Compiler_Included 1

#if defined(Y_COMPILER)
#error "Y_COMPILER shouldn't be defined here!!"
#endif

#if defined(__GNUC__) && !defined(__clang__) && !defined(__ICC)
#define Y_GNU       1
#define Y_COMPILER "gnu"
#define Y_MAJOR __GNUC__
#define Y_MINOR __GNUC_MINOR__
#define Y_PATCH __GNUC_PATCHLEVEL__
#endif

#if defined(__clang__)
#define Y_CLANG     1
#define Y_COMPILER "clang"
#define Y_MAJOR __clang_major__
#define Y_MINOR __clang_minor__
#define Y_PATCH __clang_patchlevel__
#endif

#if defined(_MSC_VER)
#define Y_MICROSOFT 1
#define Y_COMPILER "microsoft"
#define Y_MAJOR _MSC_VER
#define Y_MINOR 0
#define Y_PATCH 0
#endif

#if defined(__ICC)
#define Y_INTEL    1
#define Y_COMPILER "intel"
#define Y_MAJOR __ICC
#define Y_MINOR 0
#define Y_PATCH 0
#endif


#if !defined(Y_COMPILER)
#error "Y_COMPILER was not found"
#endif

#endif

