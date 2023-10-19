#include "y/chem/plexus/cluster.hpp"
#include "y/associative/address-book.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Cluster:: createCons(XMLog &xml)
        {
            const size_t Nc = Qm.rows;
            const size_t M  = Qm.cols;
            if(Qm.rows>0)
            {
                assert(lib.size == M);
                assert(sdb->size()==M);

                Y_XML_SECTION_OPT(xml, "Conservation", " count='" << Nc << "'");
                AddressBook conserved;
                for(size_t i=1;i<=Nc;++i)
                {
                    const Readable<unsigned> &q = Qm[i];
                    Conservation             *l = Coerce(cll).pushTail( new Conservation() );
                    for(size_t j=1;j<=M;++j)
                    {
                        const unsigned nu = q[j];
                        if(nu>0)
                        {
                            const Species &sp = *((*sdb)[j]);
                            conserved |= sp;
                            l->add(nu,sp);
                        }
                    }
                    Y_XMLOG(xml, "-- " << *l);
                }
                makeCanons(xml);

                for(const SpNode *node=lib.head;node;node=node->next)
                {
                    const Species &sp = **node;
                    if(conserved.search(sp))
                    {
                        Coerce(tier->limited) << **node;
                    }
                    else
                    {
                        Coerce(tier->roaming) << **node;
                    }
                }
            }
            else
            {
                Y_XMLOG(xml,"-- No conserved species...");
                for(const SpNode *node=lib.head;node;node=node->next)
                {
                    Coerce(tier->roaming) << **node;
                }
            }

            Y_XMLOG(xml, "-- limited: " << tier->limited);
            Y_XMLOG(xml, "-- roaming: " << tier->roaming);

        }
    }

}
