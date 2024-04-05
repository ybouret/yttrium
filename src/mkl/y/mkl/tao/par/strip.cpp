#include "y/mkl/tao/par/strip.hpp"
#include <iostream>

namespace Yttrium
{
    namespace MKL
    {
        namespace Tao
        {

            std::ostream &operator<<(std::ostream &os, const Strip &s)
            {
                os << "@row=" << s.irow << ",col=" << s.icol << " : +" << s.ncol;
                return os;
            }
            
        }

    }

}
