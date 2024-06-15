
#include "y/kemp/element/add.hpp"
#include "y/system/error.hpp"
#include "y/system/exception.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Kemp
    {

        Element:: Words::  Words() noexcept : lower(0), upper(0) {}
        Element:: Words:: ~Words() noexcept {}


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
                         Element::Words    &XP,
                         const Assembly<T> &Y,
                         Element::Words    &YP)
        {

            std::cerr << "SplitWith" << std::endl;
            const size_t m = Max(X.positive,Y.positive) >> 1;

            XP.lower = makeLower(X,m);
            XP.upper = makeUpper(X,m);

            YP.lower = makeLower(Y,m);
            YP.upper = makeUpper(Y,m);

            std::cerr << "X       : " << X << std::endl;
            std::cerr << "|_lower = " << XP.lower << std::endl;
            std::cerr << "|_upper = " << XP.upper << std::endl;


            std::cerr << "Y       : " << Y << std::endl;
            std::cerr << "|_lower = " << YP.lower << std::endl;
            std::cerr << "|_upper = " << YP.upper << std::endl;


            return m;
        }


        size_t Element:: Split(Element   &X,
                               Words     &XP,
                               Element   &Y,
                               Words     &YP,
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


        template <typename CORE, typename WORD>
        Element *MergeWith(Element     &lower,
                           Element     &upper,
                           const size_t m)
        {
            // getting old value
            const Assembly<WORD> &oldSrc  = upper.get<WORD>();
            const size_t          oldPos  = oldSrc.positive; assert(0==oldPos || oldSrc.item[oldPos-1]>0);

            // creating new element by m-shifting
            const size_t          newPos  = oldPos + m;
            AutoPtr<Element>      newUpr  = new Element( newPos * sizeof(WORD), AsCapacity );
            Assembly<WORD>       &newSrc  = newUpr->get<WORD>();
            memcpy(newSrc.item+m,oldSrc.item,oldPos*sizeof(WORD));
            newSrc.positive = newPos; assert(0==newPos || newSrc.item[newPos-1]>0 );

            return Addition<CORE,WORD>::Get(lower.get<WORD>(),newSrc);
        }


        Element * Element:: Merge(Element       &lower,
                                  Element       &upper,
                                  const size_t   m,
                                  const Ops      ops)
        {
            switch(ops)
            {
                case Ops64_32: return MergeWith<uint64_t,uint32_t>(lower,upper,m);
                case Ops64_16: return MergeWith<uint64_t,uint16_t>(lower,upper,m);
                case Ops64_8:  return MergeWith<uint64_t,uint8_t>(lower,upper,m);
              
                case Ops32_16: return MergeWith<uint32_t,uint16_t>(lower,upper,m);
                case Ops32_8:  return MergeWith<uint32_t,uint8_t>(lower,upper,m);

                case Ops16_8:  return MergeWith<uint16_t,uint8_t>(lower,upper,m);

            }
            return 0;
        }


    }

}
