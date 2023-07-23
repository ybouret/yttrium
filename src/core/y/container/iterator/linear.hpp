//! \file

#ifndef Y_Iterator_Linear_Included
#define Y_Iterator_Linear_Included 1

#include "y/container/iterator/types.hpp"
#include "y/type/args.hpp"

namespace Yttrium
{

    namespace Iterating
    {
        //______________________________________________________________________
        //
        //
        //
        //! Linear iterator
        //
        //
        //______________________________________________________________________
        template <typename T, Direction WAY>
        class Linear : public Travel<WAY>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_EXPOSE(T);
            using Travel<WAY>::Way;

            //__________________________________________________________________
            //
            //
            // C++
            //! \todo transmogrify
            //
            //__________________________________________________________________

            //! setup from user's address
            inline          Linear(T *user) noexcept : addr( (MutableType *)user )
            {}

            //! cleanup
            inline virtual ~Linear() noexcept
            { addr=0; }

            //! copy
            inline Linear(const Linear &it) noexcept : addr(it.addr) {}

            //! assign
            inline Linear & operator=(const Linear &it) noexcept
            {
                addr = it.addr; return *this;
            }

            //__________________________________________________________________
            //
            //
            // Comparisons
            //
            //__________________________________________________________________

            //! equality by address
            inline friend bool operator==(const Linear &lhs, const Linear &rhs) noexcept
            { return  lhs.addr == rhs.addr; }

            //! difference by address
            inline friend bool operator!=(const Linear &lhs, const Linear &rhs) noexcept
            { return  lhs.addr != rhs.addr; }

            //__________________________________________________________________
            //
            //
            // moving
            //
            //__________________________________________________________________

            //! ++this
            inline Linear operator++() noexcept { incr(Way); return *this; }

            //! this++
            inline Linear operator++(int) noexcept
            { Linear it(*this); incr(Way); return it; }

            //! --this
            inline Linear operator--() noexcept
            { decr(Way); return *this; }

            //! ++this
            inline Linear operator--(int) noexcept
            { Linear it(*this); decr(Way); return it; }


            //! this + n
            inline friend Linear operator+(const Linear &lhs, const ptrdiff_t n) noexcept
            { Linear tmp(lhs); tmp.incr(Way,n); return tmp; }

            //! this + n
            inline friend Linear operator+(const ptrdiff_t n, const Linear &lhs) noexcept
            { Linear tmp(lhs); tmp.incr(Way,n); return tmp; }


            //! this += n
            inline Linear & operator+=(const ptrdiff_t n) noexcept
            { incr(Way,n); return *this; }


            //! this - n
            inline friend Linear operator-(const Linear &lhs, const ptrdiff_t n) noexcept
            { Linear tmp(lhs); tmp.decr(Way,n); return tmp; }


            //! n - this
            inline friend Linear operator-(const ptrdiff_t n, const Linear &lhs) noexcept
            { Linear tmp(lhs); tmp.decr(Way,n); return tmp; }

            //! this-+= n
            inline Linear & operator-=(const ptrdiff_t n) noexcept
            { decr(Way,n); return *this; }


            //! difference of iterations
            inline friend ptrdiff_t operator-(const Linear &lhs, const Linear &rhs) noexcept
            { return diff(Way,lhs,rhs); }

            //__________________________________________________________________
            //
            //
            // access
            //
            //__________________________________________________________________

            //! access
            inline Type & operator*() noexcept {
                assert(0!=addr); return *addr;
            }

            //! access, const
            inline ConstType & operator*() const noexcept {
                assert(0!=addr); return *addr;
            }

        private:
            MutableType *addr;
            inline void incr(const IsForward &) noexcept { ++addr; }
            inline void decr(const IsForward &) noexcept { --addr; }
            inline void incr(const IsForward &, const ptrdiff_t n) noexcept { addr += n; }
            inline void decr(const IsForward &, const ptrdiff_t n) noexcept { addr -= n; }

            static inline
            ptrdiff_t diff(const IsForward &, const Linear &lhs, const Linear rhs) noexcept
            {
                return static_cast<ptrdiff_t>(lhs.addr - rhs.addr);
            }


            inline void incr(const IsReverse &) noexcept { --addr; }
            inline void decr(const IsReverse &) noexcept { ++addr; }
            inline void incr(const IsReverse &, const ptrdiff_t n) noexcept { addr += n; }
            inline void decr(const IsReverse &, const ptrdiff_t n) noexcept { addr += n; }

            static inline
            ptrdiff_t diff(const IsReverse &, const Linear &lhs, const Linear rhs) noexcept
            {
                return static_cast<ptrdiff_t>(rhs.addr - lhs.addr);
            }

        };

        

    }

}

#endif

