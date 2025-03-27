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
                std::cerr << "\t---- Building Canons ----" << std::endl;
                for(const Law *law=laws.head;law;law=law->next)
                {

                    std::cerr << "using " << law->name << std::endl;
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
                            AutoPtr<Canon> lhs = popHead();

                            store.pushTail(lhs.yield());
                        }
                        swapWith(store);
                    }
                }

                std::cerr << "|canons|=" << size << std::endl;

            }

        }
    }

}


