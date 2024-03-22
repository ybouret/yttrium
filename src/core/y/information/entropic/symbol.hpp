
//! \file

#ifndef Y_Information_Entropic_Symbol_Included
#define Y_Information_Entropic_Symbol_Included 1

#include "y/data/list/raw.hpp"
#include "y/stream/bits.hpp"
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
            //! Symbol with its information
            //
            //
            //__________________________________________________________________
            class Symbol
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef RawListOf<Symbol> List;                      //!< alias
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
                Symbol(const uint16_t initCode, const uint16_t initBits) noexcept; //!< setup
                ~Symbol()                                                noexcept; //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                const char *   name()         const noexcept; //!< printable name
                void           reset()              noexcept; //!< freq=0, code=_code, bits=_bits
                uint32_t       reduceFrequency()    noexcept; //!< reduce frequency
                void           to(StreamBits &io)      const; //!< push code/bits to io
                std::ostream & display(std::ostream &) const; //!< display

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                Symbol *        next; //!< for List
                Symbol *        prev; //!< for List
                void   *        data; //!< private data
                uint32_t        freq; //!< current frequency
                uint32_t        code; //!< current code
                uint32_t        bits; //!< current bits
                const uint16_t _code; //!< original code
                const uint16_t _bits; //!< original bits

                typedef UnsignedInt<sizeof(freq)>::Type FreqType;              //!< alias
                static const FreqType MaxFreq = IntegerFor<FreqType>::Maximum; //!< alias

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Symbol);
            };
            

        }

    }

}

#endif
