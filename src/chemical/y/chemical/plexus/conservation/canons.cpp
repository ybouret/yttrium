#include "y/chemical/plexus/conservation/canons.hpp"
#include "y/text/plural.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            Canons:: ~Canons() noexcept
            {
            }

            Canons:: Canons(XMLog &            xml,
                            const ListOf<Law> &laws,
                            const EList       &definite,
                            const SList       &unbounded) :
            CxxListOf<Canon>()
            {
                Y_XML_SECTION_OPT(xml, "Conservations::Canons", "|laws|=" << laws.size);
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

                Y_XML_COMMENT(xml, "compiling " << size << " canon" << Plural::s(size));
                for(Canon *canon=head;canon;canon=canon->next)
                    canon->compile(xml,definite,unbounded);


            }

        }
    }

}


