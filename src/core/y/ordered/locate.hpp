
//! \file

#ifndef Y_Ordered_Locate_Included
#define Y_Ordered_Locate_Included 1

#include "y/type/signs.hpp"

namespace Yttrium
{

    namespace Core
    {

        //! locate existing/would-be target in source array
        /**
         \param position where target is/should be inserted
         \param target   user's target object
         \param source   source [1..size()]
         \param compare  compare(target,source[...])
         \param return   true is found at position, false and where to insert
         */
        template <
        typename TARGET,
        typename SOURCE,
        typename COMPARE>
        bool   Locate(size_t  &position,
                      TARGET  &target,
                      SOURCE  &source,
                      COMPARE &compare)
        {
            const size_t n = source.size();

            switch(n)
            {
                case 0: position = 1; return false;
                case 1:
                    switch(compare(target,source[1]))
                    {
                        case Negative: position = 1; return false; // target <  source[1]
                        case __Zero__: position = 1; return true;  // target == source[1]
                        case Positive: position = 2; return false; // target >  source[1]
                    }
                default:
                    break;
            }
            assert(n>=2);

            switch(compare(target,source[1]))
            {
                case Negative: position = 1; return false; // target <  source[1]
                case __Zero__: position = 1; return true;  // target == source[1]
                case Positive: break;                      // target >  source[1]
            }

            switch(compare(target,source[n]))
            {
                case Negative: break;                        // target < source[n]
                case __Zero__: position = n;   return true;  // target == source[n]
                case Positive: position = n+1; return false; // target > source[n]
            }

            // initialize bisection
            size_t lower = 1;
            size_t upper = n;

        PROBE:
            const size_t probe = (lower+upper) >> 1;

            switch(compare(target,source[probe]))
            {
                case Negative: upper    = probe; goto  PROBE;
                case Positive: lower    = probe; goto  PROBE;
                case __Zero__: position = probe; return true;
            }
            assert(lower<upper);
            if(upper-lower>1) goto PROBE;
            position = lower;
            return false;
        }

    }
}

#endif

