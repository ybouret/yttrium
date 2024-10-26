//! \file

#ifndef Y_Lingo_CharDB_Included
#define Y_Lingo_CharDB_Included 1

#include "y/config/starting.hpp"
#include "y/ostream-proto.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        class Pattern;
        //______________________________________________________________________
        //
        //
        //
        //! Compact Database of Chars
        //
        //
        //______________________________________________________________________
        class CharDB
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const unsigned BITS  = 256;       //!< alias
            static const unsigned WORDS = BITS / 8;  //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit CharDB() noexcept;                  //!< setup empty
            virtual ~CharDB() noexcept;                  //!< clear
            CharDB(const CharDB &) noexcept;             //!< copy
            CharDB & operator=(const CharDB &) noexcept; //!< assign
            Y_OSTREAM_PROTO(CharDB);                     //!< display using compile()

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            CharDB & free() noexcept; //!< free
            CharDB & fill() noexcept; //!< fill
            CharDB & set(const uint8_t ibit)       noexcept; //!< set  ibit
            CharDB & clr(const uint8_t ibit)       noexcept; //!< clear ibit
            bool     has(const uint8_t ibit) const noexcept; //!< check ibit
            CharDB & operator += (const CharDB &)  noexcept; //!< merge content
            CharDB & operator -= (const CharDB &)  noexcept; //!< remove content

            CharDB & inv()                         noexcept; //!< bit-wise inversion
            size_t   size()                  const noexcept; //!< sum all active bits
            Pattern *compile()                        const; //!< simplification to Or'd patterns





        private:
            uint8_t words[WORDS];
        };
    }

}

#endif

