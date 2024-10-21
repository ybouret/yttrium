#include "y/lingo/pattern/char-db.hpp"
#include "y/calculus/bit-count.hpp"
#include <cstring>
#include <iostream>

namespace Yttrium
{
    namespace Lingo
    {

        CharDB:: CharDB() noexcept : words()
        {
            free();
        }

        CharDB:: ~CharDB() noexcept
        {
            free();
        }

        CharDB & CharDB:: free() noexcept
        {
            memset(words,0,sizeof(words));
            return *this;
        }

        CharDB & CharDB:: fill() noexcept
        {
            memset(words,0xFF,sizeof(words));
            return *this;
        }

        CharDB:: CharDB(const CharDB &db) noexcept : words()
        {
            memcpy(words,db.words,sizeof(words));
        }

        CharDB & CharDB:: operator=(const CharDB &db) noexcept
        {
            memmove(words,db.words,sizeof(words));
            return *this;
        }

        static const uint8_t theBit[8] =
        {
            0x01,
            0x02,
            0x04,
            0x08,
            0x10,
            0x20,
            0x40,
            0x80
        };

        CharDB & CharDB:: set(const uint8_t ibit) noexcept
        {
            words[ibit>>3] |= theBit[ibit&7];
            return *this;
        }

        bool CharDB:: has(const uint8_t ibit) const noexcept
        {
            return 0 != ( words[ibit>>3] & theBit[ibit&7] );
        }

        static const uint8_t theMsk[8] =
        {
            0xff-0x01,
            0xff-0x02,
            0xff-0x04,
            0xff-0x08,
            0xff-0x10,
            0xff-0x20,
            0xff-0x40,
            0xff-0x80
        };

        CharDB & CharDB:: clr(const uint8_t ibit) noexcept
        {
            words[ibit>>3] &= theMsk[ibit&7];
            return *this;
        }


        CharDB & CharDB:: operator += (const CharDB &db)    noexcept
        {
            for(size_t i=0;i<WORDS;++i)
            {
                words[i] |= db.words[i];
            }
            return *this;
        }

        size_t CharDB:: size() const noexcept
        {
            size_t ans = 0;
            for(size_t i=0;i<WORDS;++i)
            {
                ans += BitCount::Table[ words[i] ];
            }
            return ans;
        }


    }

}
