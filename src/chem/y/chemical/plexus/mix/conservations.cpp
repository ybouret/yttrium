#include "y/chemical/plexus/mix.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Mix:: buildConservations(XMLog &xml)
        {
            Y_XML_SECTION(xml, "Mix::Conservations");
            Coerce(auth) = new Conservation::Authority(*this,xml);


            for(const Conservation::Law *law=auth->laws->head;law;law=law->next)
            {
                for(const Actor *a=(*law)->head;a;a=a->next)
                {
                    Coerce(conserved.book) |= a->sp;
                }
            }

            Coerce(conserved).compile();
            Y_XMLOG(xml,"conserved=" << conserved.list);

            for(const SNode *sn=my.species.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                if(conserved.book.has(sp)) continue;
                Coerce(unbounded.book) += sp;
            }
            Coerce(unbounded).compile();
            Y_XMLOG(xml,"unbounded=" << unbounded.list);

        }
    }

}
