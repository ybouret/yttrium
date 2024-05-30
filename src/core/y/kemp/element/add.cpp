
#include "y/kemp/element.hpp"
#include "y/type/utils.hpp"
#include "y/ptr/auto.hpp"
#include "y/system/wtime.hpp"

namespace Yttrium
{
    namespace Kemp
    {


        template <typename CORE,typename WORD>
        static inline
        size_t AddAssembly(Assembly<WORD>       &sum,
                           const Assembly<WORD> &lhs,
                           const Assembly<WORD> &rhs) noexcept
        {
            Y_STATIC_CHECK(sizeof(CORE)>sizeof(WORD),BadSetup);
            assert(0==sum.positive);
            WORD * const s     = sum.item;
            CORE         carry = 0;

            // select big/little
            const Assembly<WORD> *big = &lhs;
            const Assembly<WORD> *lit = &rhs;
            if(big->positive<lit->positive) Swap(lit,big);
            assert(lit->positive<=big->positive);

            const WORD * const a = lit->item;
            const size_t       p = lit->positive;

            const WORD * const b = big->item;
            const size_t       q = big->positive;

            assert(p<=q);
            assert(sum.capacity>q);

            // sum over shared size
            for(size_t i=0;i<p;++i)
            {
                carry  += static_cast<CORE>(a[i]) + static_cast<CORE>(b[i]);
                s[i]    = static_cast<WORD>(carry);
                carry >>= Assembly<WORD>::WordBits;
            }

            // sum over remaining size
            for(size_t i=p;i<q;++i)
            {
                carry += static_cast<CORE>(b[i]);
                s[i]   = static_cast<WORD>(carry);
                carry >>= Assembly<WORD>::WordBits;
            }

            // finalize
            s[q] = static_cast<WORD>(carry);
            sum.positive = q+1;


            return sum.updateBits();
        }


#define TMX_INI() const uint64_t   t = 0!=tmx ? WallTime::Ticks() : 0
#define TMX_END() if(0!=tmx) *tmx += WallTime::Ticks() - t

        namespace
        {
            template <typename ARG>
            static inline Element *CreateAdd(const ARG &l, const ARG &r)
            {
                return new Element( (Max(l.positive,r.positive)+1) * ARG::WordSize, AsCapacity);
            }

            template <typename CORE>
            struct BuildAdd
            {
                template <typename ARG> static inline
                size_t For(ARG &s, const ARG &l, const ARG &r)
                {
                    return AddAssembly<CORE,typename ARG::WordType>(s,l,r);
                }
            };


        }


        // CORE = 64 bits
        template <>
        Element * Element:: Add<uint64_t,uint32_t>(Element &        lhs,
                                                   Element &        rhs,
                                                   uint64_t * const tmx)
        {
            const Num32     &l = lhs.get<uint32_t>();
            const Num32     &r = rhs.get<uint32_t>();
            AutoPtr<Element> s = CreateAdd(l,r);

            TMX_INI();
            s->bits = BuildAdd<uint64_t>::For(s->get<uint32_t>(),l,r);
            //s->bits = AddAssembly<uint64_t,uint32_t>(s->set(AsNum32).num32,l,r);
            TMX_END();

            return s.yield();
        }

        template <>
        Element * Element:: Add<uint64_t,uint16_t>(Element &        lhs,
                                                   Element &        rhs,
                                                   uint64_t * const tmx)
        {
            const Num16     &l = lhs.set(AsNum16).num16;
            const Num16     &r = rhs.set(AsNum16).num16;
            AutoPtr<Element> s = CreateAdd(l,r);

            TMX_INI();
            s->bits = AddAssembly<uint64_t,uint16_t>(s->set(AsNum16).num16,l,r);
            TMX_END();
            return s.yield();
        }
        


        template <>
        Element * Element:: Add<uint64_t,uint8_t>(Element &        lhs,
                                                  Element &        rhs,
                                                  uint64_t * const tmx)
        {
            const Bytes     &l = lhs.set(AsBytes).bytes;
            const Bytes     &r = rhs.set(AsBytes).bytes;
            AutoPtr<Element> s = CreateAdd(l,r); assert(AsBytes==s->state);

            TMX_INI();
            s->bits = AddAssembly<uint64_t,uint8_t>(s->bytes,l,r);
            TMX_END();
            return s.yield();
        }


        // CORE = 32 bits

        template <>
        Element * Element:: Add<uint32_t,uint16_t>(Element &        lhs,
                                                   Element &        rhs,
                                                   uint64_t * const tmx)
        {
            const Num16     &l = lhs.set(AsNum16).num16;
            const Num16     &r = rhs.set(AsNum16).num16;
            AutoPtr<Element> s = CreateAdd(l,r);

            TMX_INI();
            s->bits = AddAssembly<uint32_t,uint16_t>(s->set(AsNum16).num16,l,r);
            TMX_END();
            return s.yield();
        }

        template <>
        Element * Element:: Add<uint32_t,uint8_t>(Element &        lhs,
                                                  Element &        rhs,
                                                  uint64_t * const tmx)
        {
            const Bytes     &l = lhs.set(AsBytes).bytes;
            const Bytes     &r = rhs.set(AsBytes).bytes;
            AutoPtr<Element> s = CreateAdd(l,r); assert(AsBytes==s->state);

            TMX_INI();
            s->bits = AddAssembly<uint32_t,uint8_t>(s->bytes,l,r);
            TMX_END();
            return s.yield();
        }

        // CORE = 16 bits
        template <>
        Element * Element:: Add<uint16_t,uint8_t>(Element &        lhs,
                                                  Element &        rhs,
                                                  uint64_t * const tmx)
        {
            const Bytes     &l = lhs.set(AsBytes).bytes;
            const Bytes     &r = rhs.set(AsBytes).bytes;
            AutoPtr<Element> s = CreateAdd(l,r); assert(AsBytes==s->state);

            TMX_INI();
            s->bits = AddAssembly<uint16_t,uint8_t>(s->bytes,l,r);
            TMX_END();
            return s.yield();
        }

        

    }

}
