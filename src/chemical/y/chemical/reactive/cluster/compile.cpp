
#include "y/chemical/reactive/cluster.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        void Cluster:: compile(XMLog &xml)
        {
            Y_XML_SECTION_OPT(xml,"Cluster", " localEqs=" << size);

            //------------------------------------------------------------------
            // sort equilibria and assign sub-level
            //------------------------------------------------------------------
            LightSort::MakeSubLevel<EList>(*this);

            //------------------------------------------------------------------
            // format helper and collect species
            //------------------------------------------------------------------
            AddressBook book;
            for(const ENode *en=head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                Coerce(eqfmt).updateWith(eq);
                Coerce(eqfmt).modernizeWith(eq);
                eq.recordSpeciesInto(book);
            }

            {
                Y_XML_SECTION(xml,"Components");
                if(xml.verbose)
                {
                    for(const ENode *en=head;en;en=en->next)
                    {
                        const Equilibrium &eq = **en;
                        eqfmt.display( xml(), eq ) << " top=" << eq.indx[TopLevel] << " sub=" << eq.indx[SubLevel] << std::endl;
                    }
                }
            }

            //------------------------------------------------------------------
            // gather species
            //------------------------------------------------------------------
            for(AddressBook::Iterator it = book.begin(); it != book.end(); ++it)
            {
                const Species &sp = *static_cast<const Species *>(*it);
                Coerce(species) << sp;
            }

            LightSort::MakeSubLevel<SList>( Coerce(species) );
            for(const SNode *sn=species.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                Coerce(spfmt).updateWith(sp);
            }

            {
                Y_XML_SECTION(xml,"Species");
                if(xml.verbose)
                {
                    for(const SNode *sn=species.head;sn;sn=sn->next)
                    {
                        const Species &sp = **sn;
                        spfmt.pad( xml() << sp, sp ) << " top=" << sp.indx[TopLevel] << " sub=" << sp.indx[SubLevel] << std::endl;
                    }
                }
            }

            //------------------------------------------------------------------
            // create topology
            //------------------------------------------------------------------
            assert(size>0);
            assert(species.size>0);
            const size_t n = size;
            const size_t m = species.size;
            Coerce(nu).make(n,m);
            for(const ENode *en=head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                eq.fill(Coerce(nu)[eq.indx[SubLevel]],SubLevel);
            }
            Y_XMLOG(xml,"nu=" << nu);
        }
    }

}

