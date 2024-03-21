
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
            
            //__________________________________________________________________
            //
            //
            //
            //! Unit of an Alphabet
            //
            //
            //__________________________________________________________________
            class Unit
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef RawListOf<Unit> List;                        //!< alias
                static const uint16_t Encoding = 256;                //!< Encoding bytes
                static const uint16_t Controls = 2;                  //!< Control codes
                static const uint16_t Universe = Encoding+Controls;  //!< all possibilities
                static const uint16_t MaxAlive = Universe-1;         //!< Encoding+EOS
                static const uint16_t NYT      = Encoding;           //!< Not Yet Transmitted
                static const uint16_t EOS      = Encoding+1;         //!< End Of Stream

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                Unit(const uint16_t, const uint16_t) noexcept; //!< setup code+bits
                ~Unit() noexcept;                              //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                void           reset()              noexcept; //!< reset code/bits/fres
                void           reduceFrequency()    noexcept; //!< reduce frequency
                const char *   name()         const noexcept; //!< printable name
                void           to(StreamBits &)        const; //!< push(code,bits)
                std::ostream & display(std::ostream &) const; //!< display info

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
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



        }

    }

}

#endif

