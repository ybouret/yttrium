
#ifndef Y_Kemp_Element_TMX_Included
#define Y_Kemp_Element_TMX_Included 1

#include "y/system/wtime.hpp"

//! initialize timing
#define Y_Kemp_TMX_Ini() const uint64_t ini64 = WallTime::Ticks()

//! update timing tmx
#define Y_Kemp_TMX_Add() tmx += (WallTime::Ticks()-ini64)

#endif

