#include "y/chemical/plexus/conservation/canons.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            Canons:: ~Canons() noexcept
            {
            }

            Canons:: Canons(const ListOf<Law> &laws) :
            CxxListOf<Canon>()
            {
                for(const Law *law=laws.head;law;law=law->next)
                {

                    for(Canon *canon=head;canon;canon=canon->next)
                    {
                        assert(!canon->has(*law));

                        if(canon->accepts(*law))
                        {
                            *canon << *law;
                            goto FUSION;
                        }
                    }

                    // not found => new canon
                    pushTail( new Canon(*law) );
                    continue;

                FUSION:
                    {
                        CxxListOf<Canon> store;
                        while(size>0)
                        {
                            AutoPtr<Canon> rhs = popHead();
                            for(Canon *lhs=store.head;lhs;lhs=lhs->next)
                            {
                                if(lhs->accepts(*rhs))
                                {
                                    lhs->mergeTail(*rhs);
                                    rhs.erase();
                                    break;
                                }
                            }

                            if(rhs.isValid())
                                store.pushTail(rhs.yield());
                        }
                        swapWith(store);
                    }
                }

                for(Canon *canon=head;canon;canon=canon->next)
                {
                    canon->update();
                }

            }

        }
    }

}


