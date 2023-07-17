//! \file

#ifndef Yttrium_Config_Shallow_Included
#define Yttrium_Config_Shallow_Included 1

#include "y/config/starting.hpp"


#define Y_SHALLOW_DECL(CLASS) \
struct       CLASS##_ {};     \
extern const CLASS##_ CLASS

#define Y_SHALLOW_IMPL(CLASS) \
const CLASS##_ CLASS = {}

#endif
