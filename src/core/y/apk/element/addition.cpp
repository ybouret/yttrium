
#include "y/apk/element.hpp"
#include "y/system/error.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace APK
    {

        template <typename CoreType,typename WordType> static inline
        Element * Add(const WordType * lhs, size_t nl,
                      const WordType * rhs, size_t nr)
        {
            static const size_t   WordSize = sizeof(WordType);
            static const unsigned WordBits = WordSize * 8;

            assert(0!=lhs);
            assert(0!=rhs);
            if(nl<nr)
            {
                Swap(lhs,rhs);
                Swap(nl,nr);
            }
            assert(nl>=nr);
            const size_t     numWords = nl+1;
            const size_t     numBytes = numWords*WordSize;
            AutoPtr<Element> el    = new Element(numBytes);
            CoreType         carry = 0;
            WordType        *sum   = static_cast<WordType *>(el->entry);

            // share part
            for(size_t i=0;i<nr;++i)
            {
                carry += CoreType(lhs[i]) + CoreType(rhs[i]);
                sum[i] = static_cast<WordType>(carry);
                carry >>= WordBits;
            }

            // extra part
            for(size_t i=nr;i<nl;++i)
            {
                carry += CoreType(lhs[i]);
                carry >>= WordBits;
            }

            // carry
            sum[nl] = static_cast<WordType>(carry);

            Coerce(el->state) = Element::State[ iLog2<WordSize>::Value ];
            el->upgrade();

            return 0;

        }

        Element * Add(Element &lhs, Element &rhs)
        {

            return 0;
        }

    }

}

