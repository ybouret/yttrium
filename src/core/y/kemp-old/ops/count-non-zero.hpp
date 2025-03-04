
//! \file

#ifndef Y_Kemp_Count_Non_Zero_Included
#define Y_Kemp_Count_Non_Zero_Included 1

#include "y/kemp/rational.hpp"


namespace Yttrium
{
    namespace Kemp
    {
        //______________________________________________________________________
        //
        //
        //! count number of non-zero elements
        //
        //______________________________________________________________________
        struct CountNonZero
        {

            //! in an array-like source
            template <typename SOURCE>
            static inline size_t In(SOURCE &source)
            {
                size_t count = 0;
                for(size_t i=source.size();i>0;--i) { if(0!=source[i]) ++count; }
                return count;
            }
        };

    }

}

#endif

