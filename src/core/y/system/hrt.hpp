//! \file

#ifndef Y_System_HRT_Included
#define Y_System_HRT_Included 1

#include "y/config/starting.hpp"
#include "y/ostream-proto.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Human Readable Time
    //
    //
    //__________________________________________________________________________
    class HRT
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const unsigned Minute     = 60;            //!< seconds/minute
        static const unsigned Hour       = 60 * Minute;   //!< seconds/hour
        static const unsigned Day        = 24 * Hour;     //!< seconds/Day
        static const unsigned MaxDays    = 99;            //!< max days to display
        static const unsigned MaxSeconds = MaxDays * Day; //!< max seconds to convert
        static const size_t   Length     = 4 * 2 + 3;     //!< output width

        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        ~HRT()              noexcept; //!< cleanup
        HRT(const HRT &)    noexcept; //!< copy
        HRT(double seconds) noexcept; //!< convert from seconds
        Y_OSTREAM_PROTO(HRT);         //!< display

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! access to check/debug
        const char * operator*() const noexcept;

        
    private:
        Y_DISABLE_ASSIGN(HRT);
        char fmt[16];

    };
}

#endif

