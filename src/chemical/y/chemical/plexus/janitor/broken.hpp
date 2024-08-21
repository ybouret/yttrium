
//! \file

#ifndef Y_Chemical_Broken_Included
#define Y_Chemical_Broken_Included 1

#include "y/chemical/plexus/cluster/conservation/law.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Broken law with context
        //
        //
        //______________________________________________________________________
        class Broken
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxSeries<Broken,XMemory> Series; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Broken(const Conservation::Law &, XWritable &) noexcept; //!< init
            Broken(const Broken &broken)                   noexcept; //!< copy
            ~Broken()                                      noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Broken);                                 //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! compare by gain
            static int Compare(const Broken &, const Broken &) noexcept;

            //! compute for law
            bool       still(const XReadable &C, const Level L, XAdd &xadd);

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            xreal_t                  gain; //!< current gain
            const Conservation::Law &claw; //!< studied conservation
            XWritable               &cfix; //!< persistent SubLevel fixed

        private:
            Y_DISABLE_ASSIGN(Broken);
        };
    }

}

#endif

