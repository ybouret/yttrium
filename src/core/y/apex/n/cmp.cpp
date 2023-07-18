
#include "y/apex/natural.hpp"
#include "y/apex/n/cxx.hpp"
namespace Yttrium
{
    namespace Apex
    {
        //----------------------------------------------------------------------
        //
        //
        // comparison
        //
        //
        //----------------------------------------------------------------------

        SignType Natural:: Compare(const Natural &lhs, const Natural &rhs) noexcept
        {
            return Prototype::Compare(CONST_PROTO(lhs.impl),CONST_PROTO(rhs.impl));
        }

        //----------------------------------------------------------------------
        //
        // equal
        //
        //----------------------------------------------------------------------
        bool operator==(const Natural &lhs, const Natural &rhs) noexcept
        {
            return Prototype::AreEqual(CONST_PROTO(lhs.impl),CONST_PROTO(rhs.impl));
        }

        bool operator==(const Natural &lhs, const uint64_t rhs) noexcept
        {
            return __Zero__ == Prototype::Compare(CONST_PROTO(lhs.impl),rhs);
        }

        bool operator==(const uint64_t lhs, const Natural &rhs) noexcept
        {
            return __Zero__ == Prototype::Compare(lhs,CONST_PROTO(rhs.impl));
        }

        //----------------------------------------------------------------------
        //
        // different
        //
        //----------------------------------------------------------------------
        bool operator!=(const Natural &lhs, const Natural &rhs) noexcept
        {
            return Prototype::AreDifferent(CONST_PROTO(lhs.impl),CONST_PROTO(rhs.impl));
        }

        bool operator!=(const Natural &lhs, const uint64_t rhs) noexcept
        {
            return __Zero__ != Prototype::Compare(CONST_PROTO(lhs.impl),rhs);
        }

        bool operator!=(const uint64_t lhs, const Natural &rhs) noexcept
        {
            return __Zero__ != Prototype::Compare(lhs,CONST_PROTO(rhs.impl));
        }

        //----------------------------------------------------------------------
        //
        // <
        //
        //----------------------------------------------------------------------
        bool operator<(const Natural &lhs, const Natural &rhs) noexcept
        {
            return Negative == Prototype::Compare(CONST_PROTO(lhs.impl),CONST_PROTO(rhs.impl));
        }

        bool operator<(const Natural &lhs, const uint64_t rhs) noexcept
        {
            return Negative == Prototype::Compare(CONST_PROTO(lhs.impl),rhs);
        }

        bool operator<(const uint64_t lhs, const Natural &rhs) noexcept
        {
            return Negative == Prototype::Compare(lhs,CONST_PROTO(rhs.impl));
        }


        //----------------------------------------------------------------------
        //
        // >
        //
        //----------------------------------------------------------------------
        bool operator>(const Natural &lhs, const Natural &rhs) noexcept
        {
            return Positive == Prototype::Compare(CONST_PROTO(lhs.impl),CONST_PROTO(rhs.impl));
        }

        bool operator>(const Natural &lhs, const uint64_t rhs) noexcept
        {
            return Positive == Prototype::Compare(CONST_PROTO(lhs.impl),rhs);
        }

        bool operator>(const uint64_t lhs, const Natural &rhs) noexcept
        {
            return Positive == Prototype::Compare(lhs,CONST_PROTO(rhs.impl));
        }

        //----------------------------------------------------------------------
        //
        // <=
        //
        //----------------------------------------------------------------------
        bool operator<=(const Natural &lhs, const Natural &rhs) noexcept
        {
            return Positive != Prototype::Compare(CONST_PROTO(lhs.impl),CONST_PROTO(rhs.impl));
        }

        bool operator<=(const Natural &lhs, const uint64_t rhs) noexcept
        {
            return Positive != Prototype::Compare(CONST_PROTO(lhs.impl),rhs);
        }

        bool operator<=(const uint64_t lhs, const Natural &rhs) noexcept
        {
            return Positive != Prototype::Compare(lhs,CONST_PROTO(rhs.impl));
        }


        //----------------------------------------------------------------------
        //
        // >=
        //
        //----------------------------------------------------------------------
        bool operator>=(const Natural &lhs, const Natural &rhs) noexcept
        {
            return Negative != Prototype::Compare(CONST_PROTO(lhs.impl),CONST_PROTO(rhs.impl));
        }

        bool operator>=(const Natural &lhs, const uint64_t rhs) noexcept
        {
            return Negative != Prototype::Compare(CONST_PROTO(lhs.impl),rhs);
        }

        bool operator>=(const uint64_t lhs, const Natural &rhs) noexcept
        {
            return Negative != Prototype::Compare(lhs,CONST_PROTO(rhs.impl));
        }


    }

}

