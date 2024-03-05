
//! \file

#ifndef Y_Information_Entropic_Unit_Included
#define Y_Information_Entropic_Unit_Included 1

#include "y/data/list/raw.hpp"
#include "y/type/ints.hpp"
#include "y/stream/bits.hpp"

namespace Yttrium
{

    namespace Information
    {

        namespace Entropic
        {

            class Unit
            {
            public:
                typedef RawListOf<Unit> List;
                static const uint16_t Encoding = 256;
                static const uint16_t Controls = 2;
                static const uint16_t Universe = Encoding+Controls;
                static const uint16_t NYT      = Encoding;
                static const uint16_t EOS      = Encoding+1;

                Unit(const uint16_t, const uint16_t) noexcept;
                ~Unit() noexcept;

                void           reset()              noexcept;
                void           reduceFrequency()    noexcept;
                const char *   name()         const noexcept;
                void           to(StreamBits &)        const;
                std::ostream & display(std::ostream &) const;

                Unit          *next; //!< for list/pool
                Unit          *prev; //!< for list
                void          *priv; //!< private field
                uint32_t       freq; //!< current frequency
                uint32_t       code; //!< current code
                uint32_t       bits; //!< current bits
                const uint16_t data; //!< original data
                const uint16_t nbit; //!< original nbit

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Unit);
            };



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

