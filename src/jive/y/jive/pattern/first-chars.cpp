
#include "y/jive/pattern/first-chars.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/text/ascii/embedding.hpp"

#include "y/object.hpp"
#include <iostream>
#include <cstring>

namespace Yttrium
{
    namespace Jive
    {
        FirstChars:: ~FirstChars() noexcept
        {
            assert(0!=field);
            Object::operator delete(field,Bytes);
            Coerce(field) = 0;
        }

        FirstChars:: FirstChars() :
        field( static_cast<uint8_t *>(Object::operator new(Bytes)) )
        {
            assert( Memory::OutOfReach::Are0(field,Bytes) );
        }

        static const uint8_t on1[8] =
        {
            0x01, 0x02, 0x04, 0x08,
            0x10, 0x20, 0x40, 0x80
        };

        void FirstChars:: include(const uint8_t b) noexcept
        {
            field[b>>3] |= on1[b&7];
        }

        static const uint8_t off1[8] =
        {
            0xff-0x01, 0xff-0x02, 0xff-0x04, 0xff-0x08,
            0xff-0x10, 0xff-0x20, 0xff-0x40, 0xff-0x80
        };

        void FirstChars:: exclude(const uint8_t b) noexcept
        {
            field[b>>3] &= off1[b&7];
        }


        void FirstChars:: include(const uint8_t a, const uint8_t b) noexcept
        {
            unsigned lower = a;
            unsigned upper = b;
            if(lower>upper) Swap(lower, upper);
            while(lower<=upper)
            {
                include(lower++);
            }
        }

        void FirstChars:: exclude(const uint8_t a, const uint8_t b) noexcept
        {
            unsigned lower = a;
            unsigned upper = b;
            if(lower>upper) Swap(lower, upper);
            while(lower<=upper)
            {
                exclude(lower++);
            }
        }

        void FirstChars:: includeAll() noexcept { memset(field, 0xff, Bytes); }
        void FirstChars:: excludeAll() noexcept { memset(field, 0x00, Bytes); }

        bool   FirstChars:: operator[](const uint8_t b) const noexcept
        {
            return 0 != (field[b>>3] & on1[b&7]);
        }

        static inline bool NextBlock(unsigned &i,
                                     unsigned &j,
                                     const FirstChars &fc) noexcept
        {
            while(i<256&&!fc[i])
                ++i;
            if(i>=256) return false;
            j=i;
            while(++j<256&&fc[j])
                ;
            return true;
        }

        std::ostream & operator<<(std::ostream &os, const FirstChars &fc)
        {
            size_t   n = 0;
            unsigned i = 0;
            unsigned j = 0;
            while( NextBlock(i,j,fc) )
            {
                os << '[';
                os << ASCII::Embedding::Char[i];
                const unsigned jm = j-1;
                if(jm>i)
                    os << '-' << ASCII::Embedding::Char[jm];
                os << ']';
                ++n;
                i=j;
            }

            if(n<=0)
                os << "[]";


            return os;
        }

        void FirstChars:: add(const FirstChars &fc) noexcept
        {
            for(unsigned i=0;i<Bits;++i)
            {
                if(fc[i]) include(i);
            }
        }

        void FirstChars:: sub(const FirstChars &fc) noexcept
        {
            for(unsigned i=0;i<Bits;++i)
            {
                if(fc[i]) exclude(i);
            }
        }

    }

}

#include "y/jive/pattern/basic/single.hpp"
#include "y/jive/pattern/basic/range.hpp"
#include "y/jive/pattern/logic/or.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Jive
    {
        void     FirstChars:: sendTo(ListOf<Pattern> &target) const
        {
            unsigned i = 0;
            unsigned j = 0;
            while( NextBlock(i,j,*this) )
            {

                const unsigned jm = j-1;
                if(jm>i)
                {
                    // range
                    //std::cerr << "fc: new range  " << i << " -> " << jm << std::endl;
                    target.pushTail( new Range(i,jm) );
                }
                else
                {
                    // single
                    //std::cerr << "fc: new single " << i << std::endl;
                    target.pushTail( new Single(i) );
                }
                i=j;
            }
        }

        Pattern * FirstChars:: makeOr() const
        {
            AutoPtr<Compound> guard = new Or();
            sendTo(guard->patterns);
            if(1==guard->patterns.size)
                return guard->patterns.popTail();
            return guard.yield();
        }
    }

}
