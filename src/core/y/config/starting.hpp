//! \file

#ifndef Yttrium_Config_Starting_Included
#define Yttrium_Config_Starting_Included 1


#include "y/config/starting.h"

#define Y_DISABLE_COPY(CLASS)   private: CLASS(const CLASS &)
#define Y_DISABLE_ASSIGN(CLASS) private: CLASS & operator=(const CLASS &)
#define Y_DISABLE_COPY_AND_ASSIGN(CLASS) \
Y_DISABLE_ASSIGN(CLASS);\
Y_DISABLE_COPY(CLASS)

namespace Yttrium
{

    template <typename T>
    inline void Swap(T &a, T &b)
    {
        T t(a);
        a = b;
        b = t;
    }

}

#endif

