
#include "y/kemp/element/add.hpp"
#include "y/kemp/element/sub.hpp"
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

            const size_t m = Max(X.positive,Y.positive) >> 1;

            XP.lower = makeLower(X,m);
            XP.upper = makeUpper(X,m);

            YP.lower = makeLower(Y,m);
            YP.upper = makeUpper(Y,m);

#if 0
            std::cerr << "X       : " << X << std::endl;
            std::cerr << "|_lower = " << XP.lower << std::endl;
            std::cerr << "|_upper = " << XP.upper << std::endl;


            std::cerr << "Y       : " << Y << std::endl;
            std::cerr << "|_lower = " << YP.lower << std::endl;
            std::cerr << "|_upper = " << YP.upper << std::endl;
#endif

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


        // source * B^m
        template <typename WORD> static inline
        Element * LeftShift(const Assembly<WORD> &source,
                            const size_t          m)
        {

            const size_t oldLength = source.positive;
            const size_t newLength = oldLength + m;
            const size_t oldLinear = oldLength * sizeof(WORD);
            const size_t newLinear = newLength * sizeof(WORD);

            AutoPtr<Element> result = new Element( newLinear, AsCapacity );
            {
                Assembly<WORD>  &target = result->get<WORD>();
                memcpy(target.item+m,source.item,oldLinear);
                target.positive = newLength;
                result->bits    = target.updateBits();
            }

            return result.yield()->revise();
        }

        //! lower + upper * B^m
        template <typename CORE, typename WORD>
        Element *DoMerge(Element     &lower,
                         Element     &upper,
                         const size_t m)
        {
            AutoPtr<Element> shift = LeftShift(upper.get<WORD>(),m);
            return Addition<CORE,WORD>::Get(lower.get<WORD>(),shift->get<WORD>());
        }




        Element * Element:: Merge(Element       &lower,
                                  Element       &upper,
                                  const size_t   m,
                                  const Ops      ops)
        {
            switch(ops)
            {
                case Ops64_32: return DoMerge<uint64_t,uint32_t>(lower,upper,m);
                case Ops64_16: return DoMerge<uint64_t,uint16_t>(lower,upper,m);
                case Ops64_8:  return DoMerge<uint64_t,uint8_t>(lower,upper,m);

                case Ops32_16: return DoMerge<uint32_t,uint16_t>(lower,upper,m);
                case Ops32_8:  return DoMerge<uint32_t,uint8_t>(lower,upper,m);

                case Ops16_8:  return DoMerge<uint16_t,uint8_t>(lower,upper,m);

            }
            return 0;
        }

    }

}

#include "y/kemp/element/kar.hpp"

namespace Yttrium
{

    namespace Kemp
    {


        template <typename CORE, typename WORD>
        Element *MergeLeftShift(const Assembly<WORD> &lower,
                                   const Assembly<WORD> &upper,
                                   const size_t          m)
        {

            AutoPtr<Element> lhs = LeftShift(lower,m);
            AutoPtr<Element> rhs = LeftShift(upper,m<<1);
            return Addition<CORE,WORD>::Get(lhs->get<WORD>(),rhs->get<WORD>());
        }




        template <typename CORE, typename WORD>
        static inline
        Element *KarMul(const Assembly<WORD> &lhs,
                        const Assembly<WORD> &rhs)
        {
            const size_t nl = lhs.positive;
            const size_t nr = rhs.positive;
            Element * (*Mul)(const Assembly<WORD> &,const Assembly<WORD> &)  = KarMul<CORE,WORD>;
            Element * (*Add)(const Assembly<WORD> &,const Assembly<WORD> &)  = Addition<CORE,WORD>::Get;
            Element * (*Sub)(const Assembly<WORD> &,const Assembly<WORD> &)  = Subtraction<CORE,WORD>::Get;
            Element * (*Mix)(Element &, Element &, const size_t)             = DoMerge<CORE,WORD>;


            //------------------------------------------------------------------
            // trivial termination
            //------------------------------------------------------------------
            if(nl<=0||nr<=0) return Element::Zero();

            //------------------------------------------------------------------
            // recursive termination
            //------------------------------------------------------------------
            assert(nl>0&&nr>0);
            if(1==nl&&1==nr)
            {
                const CORE      L = lhs.item[0]; assert(L>0);
                const CORE      R = rhs.item[0]; assert(R>0);
                Element        *P = new Element(sizeof(CORE),AsCapacity);
                Assembly<CORE> &p = P->get<CORE>();
                p.positive = 1;
                p.item[0]  = L*R;
                P->bits    = p.updateBits(); assert(P->bits>0);
                return P->revise();
            }

            //------------------------------------------------------------------
            // recursive termination
            //------------------------------------------------------------------

            //------------------------------------------------------------------
            // split assemblies @m
            //------------------------------------------------------------------
            Element::Words     w1,w2;
            const size_t       m   = SplitWith(lhs, w1, rhs, w2);
            AutoPtr<Element>  &lo1 = w1.lower,  &hi1 = w1.upper;
            AutoPtr<Element>  &lo2 = w2.lower,  &hi2 = w2.upper;

            //------------------------------------------------------------------
            // build a case
            //------------------------------------------------------------------
            static const unsigned NOP   = 0x00;
            static const unsigned LO1   = 0x01;
            static const unsigned LO2   = 0x02;
            static const unsigned HI1   = 0x04;
            static const unsigned HI2   = 0x08;
            unsigned              flag  =  NOP;
            if(lo1.isValid())     flag |=  LO1;
            if(lo2.isValid())     flag |=  LO2;
            if(hi1.isValid())     flag |=  HI1;
            if(hi2.isValid())     flag |=  HI2;


            // z0 = lo1 * lo2
            // z2 = hi1 * hi2
            // z3 = (lo1+hi1)*(lo2+hi2)
            // z1 = z3 - z0 -z2

#define Z0 Mul(lo1->get<WORD>(), lo2->get<WORD>())
#define Z2 Mul(hi1->get<WORD>(), hi2->get<WORD>())

            switch(flag)
            {

                    //----------------------------------------------------------
                    //
                    // Single: [5 cases]
                    //
                    //----------------------------------------------------------
                case NOP: return Element::Zero();                               // @ 1/16
                case LO1: return Element::Zero();                               // @ 2/16
                case LO2: return Element::Zero();                               // @ 3/16
                case HI1: return Element::Zero();                               // @ 4/16
                case HI2: return Element::Zero();                               // @ 5/16

                    //----------------------------------------------------------
                    //
                    // Pairs : [6 cases]
                    //
                    //----------------------------------------------------------

                    //----------------------------------------------------------
                    // [2/6 cases]: trivial not zero
                    //----------------------------------------------------------
                case LO1|LO2: return Z0;                                        // @ 6/16
                case HI1|HI2: {                                                 // @ 7/16
                    AutoPtr<Element> z2 = Z2;
                    return LeftShift( z2->get<WORD>(), m<<1 );
                }

                    //----------------------------------------------------------
                    // [2/6 cases]: trivial zero
                    //----------------------------------------------------------
                case LO1|HI1: return Element::Zero();                           // @ 8/16
                case LO2|HI2: return Element::Zero();                           // @ 9/16

                    //----------------------------------------------------------
                    // [2/6 cases]: simplified not zero
                    //----------------------------------------------------------
                case LO1|HI2: {                                                 // @10/16
                    AutoPtr<Element> z1 = Mul(lo1->get<WORD>(), hi2->get<WORD>());
                    return LeftShift( z1->get<WORD>(), m );
                }

                case LO2|HI1: {                                                 // @11/16
                    AutoPtr<Element> z1 = Mul(lo2->get<WORD>(), hi1->get<WORD>());
                    return LeftShift( z1->get<WORD>(), m );
                }


                    //----------------------------------------------------------
                    //
                    // Triplet: [4 cases]
                    //
                    //----------------------------------------------------------

                    //----------------------------------------------------------
                    // [2/4 cases]: with Z0
                    //----------------------------------------------------------
                case LO1|LO2|HI1: {                                             // @12/16
                    AutoPtr<Element> z0 = Z0;
                    AutoPtr<Element> z1 = Mul(lo2->get<WORD>(), hi1->get<WORD>());
                    return Mix(*z0,*z1,m);
                }

                case LO1|LO2|HI2: {                                             // @13/16
                    AutoPtr<Element> z0 = Z0;
                    AutoPtr<Element> z1 = Mul(lo1->get<WORD>(), hi2->get<WORD>());
                    return Mix(*z0,*z1,m);
                }

                    //----------------------------------------------------------
                    // [2/4 cases]: with Z2
                    //----------------------------------------------------------
                case LO1|HI1|HI2: {                                             // @14/16
                    AutoPtr<Element> z2 = Z2;
                    AutoPtr<Element> z1 = Mul(lo1->get<WORD>(), hi2->get<WORD>());
                    return MergeLeftShift<CORE,WORD>(z1->get<WORD>(), z2->get<WORD>(), m);
                }

                case LO2|HI1|HI2: {                                             // @15/16
                    AutoPtr<Element> z2 = Z2;
                    AutoPtr<Element> z1 = Mul(lo2->get<WORD>(), hi1->get<WORD>());
                    return MergeLeftShift<CORE,WORD>(z1->get<WORD>(), z2->get<WORD>(), m);
                }



                    //----------------------------------------------------------
                    //
                    // full : 1
                    //
                    //----------------------------------------------------------
                case LO1|LO2|HI1|HI2: {                                         // @16/16
                    AutoPtr<Element> z0 = Z0;
                    AutoPtr<Element> z2 = Z2;
                    AutoPtr<Element> a1 = Add(lo1->get<WORD>(),hi1->get<WORD>());
                    AutoPtr<Element> a2 = Add(lo2->get<WORD>(),hi2->get<WORD>());
                    AutoPtr<Element> z3 = Mul(a1->get<WORD>(),a2->get<WORD>());
                    AutoPtr<Element> dz = Sub(z3->get<WORD>(),z2->get<WORD>());
                    AutoPtr<Element> z1 = Sub(dz->get<WORD>(),z0->get<WORD>());
                    AutoPtr<Element> m1 = Mix(*z0,*z1,m);
                    return Mix(*m1,*z2,m<<1);
                }

                default:
                    break;

            }



            throw Specific::Exception(Element::CallSign,"Corrupted Karatsuba case");
        }




        Element *Karatsuba:: Mul(Element &lhs, Element &rhs, const Ops ops)
        {
            switch(ops)
            {
                case Ops64_32: return KarMul<uint64_t,uint32_t>(lhs.get<uint32_t>(),rhs.get<uint32_t>());
                case Ops64_16: return KarMul<uint64_t,uint16_t>(lhs.get<uint16_t>(),rhs.get<uint16_t>());
                case Ops64_8:  return KarMul<uint64_t,uint8_t>(lhs.get<uint8_t>(),rhs.get<uint8_t>());

                case Ops32_16: return KarMul<uint32_t,uint16_t>(lhs.get<uint16_t>(),rhs.get<uint16_t>());
                case Ops32_8:  return KarMul<uint32_t,uint8_t>(lhs.get<uint8_t>(),rhs.get<uint8_t>());

                case Ops16_8:  return KarMul<uint16_t,uint8_t>(lhs.get<uint8_t>(),rhs.get<uint8_t>());

            }

            return 0;
        }



    }

}
