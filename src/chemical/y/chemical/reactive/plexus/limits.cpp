
#include "y/chemical/reactive/plexus/limits.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Limits:: ~Limits() noexcept
        {
        }

        Limits:: Limits(const LimitsBank &lbank,
                        const SBank      &sbank) noexcept :
        LimitsType(lbank),
        repo(sbank)
        {
        }

        bool Limits:: contains(const Species &s) const noexcept
        {
            for(const LimitsNode *node=head;node;node=node->next)
            {
                const Limit &l = **node;
                if(l.has(s)) return true;
            }
            return false;
        }


        void Limits:: operator()(const Species &s,
                                 const xreal_t  x)
        {
            assert( !contains(s) );
            LimitsType &self = *this;
            Limit       here(s,x,repo); assert(1==here.size);
            switch(size)
            {
                case 0: self << here; break;
                case 1: {
                    Limit &mine = **head;
                    switch( Sign::Of(mine.extent,here.extent) )
                    {
                        case Negative: self << here;         break;
                        case __Zero__: mine.mergeTail(here); break;
                        case Positive: self >> here;         break;
                    }
                } break;
                default:
                    throw Exception("not implemented");
            }
        }

    }

}

