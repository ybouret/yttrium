//! \file

#ifndef Yttrium_Config_Shallow_Included
#define Yttrium_Config_Shallow_Included 1

#include "y/config/starting.hpp"

//______________________________________________________________________________
//
//
//! declare a shallow helper class
//
//______________________________________________________________________________
#define Y_SHALLOW_DECL(CLASS) \
struct       CLASS##_ {};     \
extern const CLASS##_ CLASS

//______________________________________________________________________________
//
//
//! implement a shallow helper class
//
//______________________________________________________________________________
#define Y_SHALLOW_IMPL(CLASS) \
const CLASS##_ CLASS = {}

#endif
