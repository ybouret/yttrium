
//! \file

#ifndef Y_MKL_TAO_Strip_Included
#define Y_MKL_TAO_Strip_Included 1

#include "y/config/starting.hpp"
#include <iosfwd>

namespace Yttrium
{
    namespace MKL
    {
        namespace Tao
        {

            //__________________________________________________________________
            //
            //
            //
            //! Strip matching Tiling<size_t>::Segment
            //
            //
            //__________________________________________________________________
            struct Strip
            {
                size_t icol; //!< initial column
                size_t irow; //!< initial row
                size_t ncol; //!< number of columns
                size_t cend; //!< end column

                //! display
                friend std::ostream &operator<<(std::ostream &os, const Strip &s);
            };

        }

    }

}

#endif


