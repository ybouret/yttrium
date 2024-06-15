
#include "y/kemp/element/add.hpp"
#include "y/system/error.hpp"
#include "y/system/exception.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Kemp
    {

        template <typename T>
        static inline
        Element * makeLower(const Assembly<T> &source,
                            size_t             nlower)
        {
            nlower = Min(nlower,source.positive);
            if(nlower<=0) return 0;

            const size_t toCopy  = nlower * sizeof(T);
            Element     *element = new Element( toCopy, AsCapacity );
            Assembly<T> &target  = element->get<T>();
            memcpy(target.item,source.item,toCopy);
            target.positive = nlower;
            element->bits   = target.updateBits();
            return element->revise();
        }

        template <typename T>
        static inline
        Element * makeUpper(const Assembly<T> &source,
                            const size_t       nlower)
        {
            const size_t ntotal = source.positive;
            if(nlower>=ntotal) return 0;
            const size_t nupper  = ntotal-nlower;
            const size_t toCopy  = nupper * sizeof(T);
            Element     *element = new Element( toCopy, AsCapacity );
            Assembly<T> &target  = element->get<T>();
            memcpy(target.item,source.item+nlower,toCopy);
            target.positive = nupper;
            element->bits   = target.updateBits();
            return element->revise();
        }




        template <typename T>
        static inline
        size_t SplitWith(const Assembly<T> &X,
                         Element::Pair     &XP,
                         const Assembly<T> &Y,
                         Element::Pair     &YP)
        {

            std::cerr << "SplitWith" << std::endl;
            const Assembly<T> *bigE = &X,  *litE = &Y;
            Element::Pair     *bigP = &XP, *litP = &YP;

            if(litE->positive>bigE->positive)
            {
                Swap(litE,bigE);
                Swap(litP,bigP);
            }
            assert(litE->positive<=bigE->positive);
            // std::cerr << "\tlit=" << *litE << std::endl;

            const size_t n = bigE->positive;
            const size_t m = n>>1;

            //------------------------------------------------------------------
            //
            // process big
            //
            //------------------------------------------------------------------
            {
                std::cerr << "\tbig=" << *bigE << std::endl;
                std::cerr << "\t  m=" << m << std::endl;
                bigP->lower = makeLower(*bigE,m);
                bigP->upper = makeUpper(*bigE,m);
                std::cerr << "\t\tlower=" << bigP->lower << std::endl;
                std::cerr << "\t\tupper=" << bigP->upper << std::endl;
            }

            //------------------------------------------------------------------
            //
            // process little
            //
            //------------------------------------------------------------------
            {
                std::cerr << "\tlit=" << *litE << std::endl;
                litP->lower = makeLower(*litE,m);
                litP->upper = makeUpper(*litE,m);
                std::cerr << "\t\tlower=" << litP->lower << std::endl;
                std::cerr << "\t\tupper=" << litP->upper << std::endl;
            }



            return m;
        }


        size_t Element:: Split(Element   &X,
                               Pair      &XP,
                               Element   &Y,
                               Pair      &YP,
                               const Ops &ops)
        {
            switch(ops)
            {
                case Ops64_8:
                case Ops32_8:
                case Ops16_8:
                    return SplitWith(X.get<uint8_t>(),XP,Y.get<uint8_t>(),YP);

                case Ops64_16:
                case Ops32_16:
                    return SplitWith(X.get<uint16_t>(),XP,Y.get<uint16_t>(),YP);

                case Ops64_32:
                    return SplitWith(X.get<uint32_t>(),XP,Y.get<uint32_t>(),YP);

            }
            return 0;
        }


#if 0
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

#endif

    }

}
