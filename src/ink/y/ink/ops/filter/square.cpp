#include "y/ink/ops/filter/square.hpp"
#include "y/calculus/isqrt.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Ink
    {

        namespace Crux
        {
            SquareFilter:: ~SquareFilter() noexcept {}

            static inline unit_t GetWidth(const char * const theName,
                                          const unsigned     numData)
            {
                const unsigned s = IntegerSquareRoot(numData);
                if(s*s!=numData) throw Specific::Exception("Ink::SquareFilter", "'%s' is not square", theName);
                return s;
            }

            SquareFilter:: SquareFilter(const char * const theName, const unsigned numData) :
            width_( GetWidth(theName,numData) ),
            delta_( (width_-1)>>1 )
            {
            }


        }

    }

}
