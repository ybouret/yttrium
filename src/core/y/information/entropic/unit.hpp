
//! \file

#ifndef Y_Information_Entropic_Unit_Included
#define Y_Information_Entropic_Unit_Included 1

#include "y/data/list/raw.hpp"
#include "y/type/ints.hpp"

namespace Yttrium
{


    namespace Information
    {

        namespace Entropic
        {
            //__________________________________________________________________
            //
            //
            //
            // Types
            //
            //
            //__________________________________________________________________
            typedef uint32_t Frequency; //!< hold frequencies
            typedef uint16_t Code;      //!< hold codes
            typedef uint16_t Bits;      //!< hold bits for codes

            //__________________________________________________________________
            //
            //
            //
            //! C-Style unit
            //
            //
            //__________________________________________________________________
            struct Unit
            {
                typedef RawListOf<Unit> List; //!< list of unit
                static const Frequency  MaxSumFreq = IntegerFor<Frequency>::Maximum;

                Code       code; //!< current code
                Bits       bits; //!< bits for code
                Frequency  freq; //!< frequencies
                Unit      *next; //!< for list
                Unit      *prev; //!< for list
            };
        }

    }

}

#endif

