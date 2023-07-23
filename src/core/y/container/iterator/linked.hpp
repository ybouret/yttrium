
//! \file

#ifndef Y_Iterator_Linked_Included
#define Y_Iterator_Linked_Included 1

#include "y/container/iterator/types.hpp"
#include "y/type/args.hpp"
#include "y/type/signs.hpp"

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
        template <typename T, typename NODE, Direction WAY>
        class Linked : public Travel<WAY>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_EXPOSE(T,Type);    //!< aliases
            Y_ARGS_EXPOSE(NODE,Node); //!< aliases
            using Travel<WAY>::Way;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup from user's node
            inline          Linked(NODE *user) noexcept :
            Travel<WAY>(), node( (MutableNode *)user ) {}

            //! cleanup
            inline virtual ~Linked() noexcept { node=0; }

            //! copy
            inline Linked(const Linked &other) noexcept :
            Travel<WAY>(), node(other.node) {}

            //! assign
            inline Linked & operator=(const Linked &other) noexcept
            {
                node = other.node;
                return *this;
            }

            //__________________________________________________________________
            //
            //
            // Access
            //
            //__________________________________________________________________

            //! access
            inline Type & operator*() noexcept
            { assert(node); return **node; }

            //! access, const
            inline ConstType & operator*() const noexcept
            { assert(node); return **node; }

            //__________________________________________________________________
            //
            //
            // Comparisons
            //
            //__________________________________________________________________

            //! equality by nodes
            inline friend bool operator==(const Linked &lhs, const Linked &rhs) noexcept
            { return  lhs.node == rhs.node; }

            //! difference by address
            inline friend bool operator!=(const Linked &lhs, const Linked &rhs) noexcept
            { return  lhs.node != rhs.node; }

            //__________________________________________________________________
            //
            //
            // moving
            //
            //__________________________________________________________________

            //! ++this
            inline Linked &operator++() noexcept { incr(Way); return *this; }

            //! this++
            inline Linked operator++(int) noexcept
            { Linked it(*this); incr(Way); return it; }

            //! --this
            inline Linked &operator--() noexcept
            { decr(Way); return *this; }

            //! this--
            inline Linked operator--(int) noexcept
            { Linked it(*this); decr(Way); return it; }


            //! this + n
            inline friend Linked operator+(const Linked &lhs, const ptrdiff_t n) noexcept
            { Linked tmp(lhs); tmp.incr(Way,n); return tmp; }

            //! this + n
            inline friend Linked operator+(const ptrdiff_t n, const Linked &lhs) noexcept
            { Linked tmp(lhs); tmp.incr(Way,n); return tmp; }


            //! this += n
            inline Linked & operator+=(const ptrdiff_t n) noexcept
            { incr(Way,n); return *this; }


            //! this - n
            inline friend Linked operator-(const Linked &lhs, const ptrdiff_t n) noexcept
            { Linked tmp(lhs); tmp.decr(Way,n); return tmp; }



            //! this -= n
            inline Linked & operator-=(const ptrdiff_t n) noexcept
            { decr(Way,n); return *this; }


#if 0
            //! difference of iterations
            inline friend ptrdiff_t operator-(const Linear &lhs, const Linear &rhs) noexcept
            { return diff(Way,lhs,rhs); }
#endif

        private:
            MutableNode *node;

            inline void incr(const IsForward &) noexcept { assert(node); node=node->next; }
            inline void decr(const IsForward &) noexcept { assert(node); node=node->prev; }

            inline void move(ptrdiff_t n) noexcept
            {
                switch( Sign::Of(n) )
                {
                    case Positive:
                        while(n-- > 0) { assert(node); node=node->next; }
                        break;
                    case __Zero__: break;
                    case Negative:
                        while(n++ < 0) { assert(node); node=node->prev; }
                        break;
                }
            }

            inline void incr(const IsForward &, ptrdiff_t n) noexcept
            { move(n); }

            inline void decr(const IsForward &, const ptrdiff_t n) noexcept
            { move(-n); }



            inline void incr(const IsReverse &) noexcept { assert(node); node=node->prev; }
            inline void decr(const IsReverse &) noexcept { assert(node); node=node->next; }
            inline void incr(const IsReverse &, const ptrdiff_t n) noexcept { move(-n); }
            inline void decr(const IsReverse &, const ptrdiff_t n) noexcept { move( n); }

#if 0
            static inline
            ptrdiff_t diff(const IsForward &, const Linear &lhs, const Linear rhs) noexcept
            {
                return static_cast<ptrdiff_t>(lhs.addr - rhs.addr);
            }

            static inline
            ptrdiff_t diff(const IsReverse &, const Linear &lhs, const Linear rhs) noexcept
            {
                return static_cast<ptrdiff_t>(rhs.addr - lhs.addr);
            }
#endif

        };

    }

}
#endif

