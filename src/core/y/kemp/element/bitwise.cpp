#include "y/kemp/element/add.hpp"

namespace Yttrium
{
    namespace Kemp
    {


        template <typename T> static inline
        T OpAND(const T lhs, const T rhs) noexcept
        {
            return lhs&rhs;
        }

        template <typename T> static inline
        T OpOR(const T lhs, const T rhs) noexcept
        {
            return lhs|rhs;
        }

        template <typename T> static inline
        T OpXOR(const T lhs, const T rhs) noexcept
        {
            return lhs^rhs;
        }

        template <typename T> static inline
        size_t ApplyBitwise(Logical            op,
                            Assembly<T>       &out,
                            const Assembly<T> &lhs,
                            const Assembly<T> &rhs) noexcept
        {
            const Assembly<T> *big = &lhs;
            const Assembly<T> *lit = &rhs;
            if(lit->positive>big->positive) Swap(big,lit);
            const size_t p = lit->positive;
            const size_t q = big->positive; assert(q>=p);
            T *          x = out.item;
            const T *    b = big->item;
            T (*proc)(const T, const T) = 0;
            switch(op)
            {
                case OR:  proc = OpOR<T>;  break;
                case AND: proc = OpAND<T>; break;
                case XOR: proc = OpXOR<T>; break;
            }

            {
                const T * a = lit->item;
                for(size_t i=p;i>0;--i)
                    *(x++) = proc( *(a++), *(b++) );
            }

            for(size_t i=q-p;i>0;--i)
                *(x++) = proc(0,*(b++));

            out.positive = q;
            return out.updateBits();
        }




        Element * Element:: Bitwise(Logical op, Element &lhs, Element &rhs)
        {
            AutoPtr<Element> el = 0;
            switch( TuneUp(lhs,rhs) )
            {
                case AsBytes:
                    el       = new Element( Max(lhs.bytes.positive,rhs.bytes.positive), AsCapacity); assert(AsBytes==el->state);
                    el->bits = ApplyBitwise(op, el->bytes, lhs.bytes, rhs.bytes);
                    break;

                case AsNum16:
                    el       = new Element( Max(lhs.num16.positive,rhs.num16.positive)*2, AsCapacity); assert(AsBytes==el->state);
                    el->bits = ApplyBitwise(op, el->get<uint16_t>(), lhs.num16, rhs.num16);
                    break;

                case AsNum32:
                    el       = new Element( Max(lhs.num32.positive,rhs.num32.positive)*4, AsCapacity); assert(AsBytes==el->state);
                    el->bits = ApplyBitwise(op, el->get<uint32_t>(), lhs.num32, rhs.num32);
                    break;

                case AsNum64:
                    el       = new Element( Max(lhs.num64.positive,rhs.num64.positive)*8, AsCapacity); assert(AsBytes==el->state);
                    el->bits = ApplyBitwise(op, el->get<uint64_t>(), lhs.num64, rhs.num64);
                    break;
            }
            assert(el.isValid());
            return Shrink( el.yield()->revise() );
        }

    }

}

