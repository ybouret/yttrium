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

                //--------------------------------------------------------------
                //
                // probe conservation matrix
                //
                //--------------------------------------------------------------
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

                //--------------------------------------------------------------
                //
                // dispatch species
                //
                //--------------------------------------------------------------
                Coerce(kept) = new Booleans(lib.size);
                {
                    Writable<const bool> &flag=Coerce(*kept);
                    size_t                indx=1;
                    for(const SpNode *node=lib.head;node;node=node->next,++indx)
                    {
                        const Species &sp = **node; assert(sp.indx[SubLevel]==indx);
                        if(conserved.search(sp))
                        {
                            Coerce(tier->conserved) += sp;
                            Y_XMLOG(xml, "  --> conserved " << sp);
                            Coerce(flag[indx]) = true;
                        }
                        else
                        {
                            Coerce(tier->unbounded) += sp;
                            Y_XMLOG(xml, "  --> unbounded " << sp);
                            Coerce(flag[indx]) = false;
                        }
                    }
                }
                
            }
            else
            {
                Y_XMLOG(xml,"-- No conserved species...");
                Coerce(kept) = new Booleans(lib.size,false);
                for(const SpNode *node=lib.head;node;node=node->next)
                    Coerce(tier->unbounded) += **node;
            }


        }
    }

}
