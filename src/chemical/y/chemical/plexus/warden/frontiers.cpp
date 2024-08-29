
#include "y/chemical/plexus/warden/frontiers.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        Frontiers:: Frontiers(const Fund &fund) noexcept :
        FList(fund.fbank),
        sbank(fund.sbank)
        {
        }

        Frontiers:: ~Frontiers() noexcept
        {
        }

        std::ostream & operator<<(std::ostream &os, const Frontiers &self)
        {
            const FList &F = self;
            os << "req=";
            if(F.size<=0)
                os << "none";
            else
                os << F;
            return os;
        }

        bool Frontiers:: sorted() const noexcept
        {
            if(size<=1) return true;
            for(const FNode *node=head,*next=head->next;0!=next;node=next,next=node->next)
            {
                if( (**node).xi >= (**next).xi ) return false;
            }
            return true;
        }

        FNode * Frontiers:: make(const xreal_t  xi,
                                 const Species &sp)
        {
            const Frontier  f(sbank,xi,sp);
            return proxy->produce(f);
        }


        void Frontiers:: operator()(const xreal_t  xi,
                                    const Species &sp)
        {

            // get rid of trivial cases
            switch(size)
            {
                case 0: // initialize
                    pushTail( make(xi,sp) );
                    return;

                case 1: { // three possible cases
                    Frontier &f = **head;
                    switch( Sign::Of(xi, f.xi) )
                    {
                        case Negative: pushHead( make(xi,sp) ); break;
                        case __Zero__: f << sp;                 break;
                        case Positive: pushTail( make(xi,sp) ); break;
                    }
                    assert(sorted());
                } return;

                default: // generic case
                    break;
            }
            assert(size>=2);

            // generic case
            FNode * lower = head;
            switch( Sign::Of(xi, (**lower).xi) )
            {
                case Negative: pushHead( make(xi,sp) ); assert(sorted()); return;
                case __Zero__: **lower << sp;           assert(sorted()); return;
                case Positive: break;
            }

            FNode * const upper = tail;
            switch( Sign::Of(xi, (**upper).xi) )
            {
                case Negative: break;
                case __Zero__: **upper << sp;            assert(sorted()); return;
                case Positive: pushTail( make(xi,sp) );  assert(sorted()); return;
            }

        PROBE:
            FNode *probe = lower->next;
            if(upper!=probe)
            {
                switch( Sign::Of(xi,(**probe).xi) )
                {
                    case Negative:
                        break; // after lower
                    case __Zero__: (**probe) << sp;  assert(sorted()); return;
                    case Positive:
                        lower = probe;
                        goto PROBE;
                }
            }

            insertAfter(lower, make(xi,sp) );
            assert(sorted());
        }

    }

}

