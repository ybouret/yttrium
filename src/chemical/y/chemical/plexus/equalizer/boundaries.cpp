
#include "y/chemical/plexus/equalizer/boundaries.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Banks:: Banks() : b(), s() {}

        Banks::  ~Banks() noexcept {}
    }

}

namespace Yttrium
{
    namespace Chemical
    {
        Boundaries:: Boundaries(const Banks &banks) noexcept :
        BList(banks.b),
        sbank(banks.s)
        {
        }

        Boundaries:: ~Boundaries() noexcept {}


        void Boundaries:: atTail(const xreal_t xi, const Species &sp) {
            const Boundary b(sbank,xi,sp);
            (*this) << b;
        }

        void Boundaries:: atHead(const xreal_t xi, const Species &sp) {
            const Boundary b(sbank,xi,sp);
            (*this) >> b;
        }


        void Boundaries:: operator()(const xreal_t         xi,
                                     const Species &       sp)
        {

            switch(size)
            {
                case 0: atTail(xi,sp); return;

                case 1:
                    switch( Sign::Of(xi,(**head).xi) )
                    {
                        case Negative: atHead(xi,sp);  break;
                        case Positive: atTail(xi,sp);  break;
                        case __Zero__: (**head) << sp; break;
                    }
                    return;

                default:
                    break;
            }

            // check against head
            BNode *lower = head;
            switch( Sign::Of(xi, (**lower).xi) )
            {
                case Negative: atHead(xi,sp);   return;
                case __Zero__: (**lower) << sp; return;
                case Positive:                  break;
            }

            // check againt tail
            BNode * const upper = tail; assert(upper!=lower);
            switch( Sign::Of(xi, (**upper).xi) )
            {
                case Negative: break;
                case __Zero__: (**upper) << sp; return;
                case Positive: atTail(xi,sp);   return;

            }

        PROBE:
            BNode * const probe = lower->next;
            if(upper!=probe)
            {
                switch( Sign::Of(xi, (**probe).xi) )
                {
                    case Negative: goto FOUND;
                    case __Zero__: (**probe) << sp; return;
                    case Positive: break;
                }
                lower = probe;
                goto PROBE;
            }

        FOUND:
            const Boundary b(sbank,xi,sp);
            insertAfter(lower,proxy->produce(b));
        }


        bool Boundaries:: sorted() const noexcept
        {
            if(size>1)
            {
                size_t       num = size-1;
                const BNode *lhs = head;      assert(0!=lhs);
                const BNode *rhs = lhs->next; assert(0!=rhs);
                while(num-- > 0)
                {
                    assert(0!=lhs);
                    assert(0!=rhs);
                    switch(Sign::Of( (**lhs).xi, (**rhs).xi ) )
                    {
                        case Negative: break;
                        case __Zero__:
                        case Positive:
                            return false;
                    }
                    lhs = rhs;
                    rhs = lhs->next;
                }
            }
            return true;
        }
    }

}
