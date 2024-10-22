#include "y/lingo/pattern/char-db.hpp"
#include <cstring>

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

        static const uint8_t bitsInByte[256] =
        {
#           include "bits-in-byte.hxx"
        };


        size_t CharDB:: size() const noexcept
        {
            size_t ans = 0;
            for(size_t i=0;i<WORDS;++i)
            {
                ans += bitsInByte[ words[i] ];
            }
            return ans;
        }




    }

}


#include "y/lingo/pattern/basic/all.hpp"
#include "y/lingo/pattern/logic/or.hpp"
#include "y/system/exception.hpp"
#include <iostream>

namespace Yttrium
{
    namespace Lingo
    {

        Pattern * CharDB:: compile() const
        {
            static const unsigned last = 256;
            static const char     name[] = "Lingo::CharDB.compile";

            //------------------------------------------------------------------
            //
            //
            // trivial cases
            //
            //
            //------------------------------------------------------------------
            switch( size() )
            {
                case 0:    return new Void();
                case last: return new Any1();
            }

            //------------------------------------------------------------------
            //
            //
            // at leat one non trivial interval
            //
            //
            //------------------------------------------------------------------
            Pattern::List plist;
            {
                unsigned lower = 0;

                while(lower<last)
                {
                    //----------------------------------------------------------
                    //
                    // stop at first 1
                    //
                    //----------------------------------------------------------
                    while( !has(lower) )
                    {
                        if(++lower>=last) goto DONE;
                    }

                    //----------------------------------------------------------
                    //
                    // look for next 0
                    //
                    //----------------------------------------------------------
                    unsigned upper = lower;
                    while(++upper<last && has(upper) )
                        ;

                    //----------------------------------------------------------
                    //
                    // emit
                    //
                    //----------------------------------------------------------
                    {
                        const uint8_t  lo = lower;
                        const uint8_t  up = upper-1;
                        const unsigned nn = upper-lower;
                        switch(nn)
                        {
                            case 0:  throw Specific::Exception(name, "***empty interval!");
                            case 1:  assert(lo==up); plist.pushTail( new Byte(lo)    ); break;
                            default: assert(lo<up);  plist.pushTail( new Lump(lo,up) ); break;
                        }
                    }

                    //----------------------------------------------------------
                    //
                    // next search
                    //
                    //----------------------------------------------------------
                    lower = ++upper;
                }
                DONE: ;
            }


            //------------------------------------------------------------------
            //
            //
            // comvert plist to pattern
            //
            //
            //------------------------------------------------------------------
            switch(plist.size)
            {
                case 0: throw Specific::Exception(name, "*** empty pattern list!");
                case 1: return plist.popHead();
                default:
                    break;
            }

            AutoPtr<Logic> result = new Or();
            result->swapWith(plist);
            return result.yield();
        }

        std::ostream & operator<<(std::ostream &os, const CharDB &db)
        {
            const AutoPtr<const Pattern> p = db.compile();
            return os << p->regularExpression();
        }

        Pattern * Pattern:: Among(const String &str)
        {
            const size_t n = str.size();
            if(n<=0) throw Specific::Exception("Pattern::Among","empty string!");
            CharDB db;
            for(size_t i=1;i<=n;++i)
            {
                db.set( str[i] );
            }
            return db.compile();
        }

    }

}
