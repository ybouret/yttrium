
#include "y/chemical/reactive/plexus/limits.hpp"

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
            
        }

    }

}

