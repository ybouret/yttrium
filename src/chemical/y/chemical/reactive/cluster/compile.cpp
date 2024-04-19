
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
                updateWith(eq);
                modernizeWith(eq);
                eq.recordSpeciesInto(book);
            }

            {
                Y_XML_SECTION(xml,"Components");
                if(xml.verbose)
                {
                    for(const ENode *en=head;en;en=en->next)
                    {
                        const Equilibrium &eq = **en;
                        display( xml(), eq ) << " top=" << eq.indx[TopLevel] << " sub=" << eq.indx[SubLevel] << std::endl;
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

        }
    }

}

