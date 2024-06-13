
#include "y/kemp/element/add.hpp"
#include "y/system/error.hpp"
#include "y/system/exception.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Kemp
    {

        template <typename T> static inline
        size_t Split(AutoPtr<Element>  &lower,
                     AutoPtr<Element>  &upper,
                     const Assembly<T> &source)
        {

            const size_t np = source.positive;
            const size_t nl = np >> 1;
            const size_t nu = np-nl;

            lower = new Element( nl * sizeof(T), AsCapacity);
            upper = new Element( nu * sizeof(T), AsCapacity);

            Assembly<T> &L = lower->get<T>();
            Assembly<T> &U = upper->get<T>();

            for(size_t i=0;i<nl;++i)
                L.item[i] = source.item[i];
            L.positive  = nl;
            lower->bits = L.updateBits();
            lower->revise();

            for(size_t i=0;i<nu;++i)
                U.item[i] = source.item[nl+i];
            U.positive = nu;
            upper->bits = U.updateBits();
            upper->revise();

            return nl;

        }

        size_t Element:: split(AutoPtr<Element> &lower,
                               AutoPtr<Element> &upper) const
        {
            switch( state )
            {
                case AsBytes: return Split(lower,upper,bytes);
                case AsNum16: return Split(lower,upper,num16);
                case AsNum32: return Split(lower,upper,num32);
                case AsNum64: return Split(lower,upper,num64);
            }
            throw Specific::Exception(CallSign, "corrupted state to split");
        }


        template <typename CORE, typename WORD> static inline
        Element * Merge_(const Assembly<WORD> &lower,
                         const Assembly<WORD> &upper,
                         const size_t          m)
        {
            const size_t     newPositive = upper.positive + m;
            AutoPtr<Element> newElement = new Element( newPositive * sizeof(WORD), AsCapacity );
            Assembly<WORD>  &newUpper   = newElement->get<WORD>();
            const size_t     upperPos   = upper.positive;
            newUpper.positive = m + upperPos;
            memcpy(newUpper.item+m,upper.item,upperPos * sizeof(WORD));

            return Addition<CORE,WORD>::Get(lower,newUpper);

        }


        Element * Element:: Merge(const Ops   ops,
                                  Element    &lower,
                                  Element     &upper,
                                  const size_t m)
        {

            switch(ops)
            {
                case Ops64_32: return Merge_<uint64_t,uint32_t>(lower.get<uint32_t>(), upper.get<uint32_t>(),m);
                case Ops64_16: return Merge_<uint64_t,uint16_t>(lower.get<uint16_t>(), upper.get<uint16_t>(),m);
                case Ops64_8:  return Merge_<uint64_t,uint8_t>(lower.get<uint8_t>(), upper.get<uint8_t>(),m);

                case Ops32_16: return Merge_<uint32_t,uint16_t>(lower.get<uint16_t>(), upper.get<uint16_t>(),m);
                case Ops32_8:  return Merge_<uint32_t,uint8_t>(lower.get<uint8_t>(), upper.get<uint8_t>(),m);

                case Ops16_8:  return Merge_<uint16_t,uint8_t>(lower.get<uint8_t>(), upper.get<uint8_t>(),m);
            }
            throw Specific::Exception(CallSign, "corrupted ops to merge");
        }
    }

}
