
//! \file

#ifndef Y_IO_Stock_Included
#define Y_IO_Stock_Included 1

#include "y/stream/io/char.hpp"
#include "y/data/pool/cxx.hpp"

namespace Yttrium
{
    namespace IO
    {
        //______________________________________________________________________
        //
        //
        //
        //! Versatile stock of chars
        //
        //
        //______________________________________________________________________
        class Stock : public CxxPoolOf<Char>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Stock() noexcept; //!< setup empty
            virtual ~Stock() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void     reserve(size_t); //!< reserve extra chars

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Stock);
        };
    }

}

#endif
