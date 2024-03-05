
//! \file

#ifndef Y_Information_Entropic_Unit_Included
#define Y_Information_Entropic_Unit_Included 1

#include "y/data/list/raw.hpp"
#include "y/type/ints.hpp"
#include "y/ostream-proto.hpp"

namespace Yttrium
{


    namespace Information
    {

        namespace Entropic
        {

#if 0
            //__________________________________________________________________
            //
            //
            //
            // Types
            //
            //
            //__________________________________________________________________
            typedef uint32_t Frequency; //!< hold frequencies


            //__________________________________________________________________
            //
            //
            //
            //! Control codes offset
            //
            //
            //__________________________________________________________________
            enum ControlOffset
            {
                ControlNYT = 0,  //!< for Not Yet Transmitted
                ControlEOS = 1   //!< for End Of Stream

            };


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
                typedef RawListOf<Unit> List;                              //!< list of unit
                static const unsigned   Encoding   = 256;                  //!< Bytes
                static const unsigned   Controls   = 2;                    //!< Controls
                static const unsigned   Universe   = Encoding+Controls;    //!< all possible
                static const unsigned   MaxUsed    = Universe-1;           //!< with removed NYT
                static const unsigned   NYT        = Encoding+ControlNYT;  //!< index of NYT
                static const unsigned   EOS        = Encoding+ControlEOS;  //!< index of EOS

                static const Frequency  MaxSumFreq = IntegerFor<Frequency>::Maximum;

                unsigned   code; //!< current code
                unsigned   bits; //!< bits for code
                Frequency  freq; //!< frequencies
                Unit      *next; //!< for list
                Unit      *prev; //!< for list
                void      *priv; //!< private data
                unsigned   byte; //!< original data

                Y_OSTREAM_PROTO(Unit);
            };
#endif
        }

    }

}

#endif

