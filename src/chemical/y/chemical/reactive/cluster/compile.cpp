
#include "y/chemical/reactive/cluster.hpp"
#include "y/mkl/algebra/rank.hpp"
#include "y/system/exception.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        void Cluster:: compile(Equilibria &eqs, const Readable<XReal> &topK, XMLog &xml)
        {
            static const char here[] = "Chemical::Cluster";
            Y_XML_SECTION_OPT(xml,here, " localEqs=" << size);

            //------------------------------------------------------------------
            //
            // sort equilibria and assign sub-level
            //
            //------------------------------------------------------------------
            LightSort::MakeSubLevel<EList>(*this);

            //------------------------------------------------------------------
            //
            // format helpers and collect species
            //
            //------------------------------------------------------------------
            {
                AddressBook book;
                for(const ENode *en=head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    Coerce(eqfmt).upgradeWith(eq);
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

            
                for(AddressBook::Iterator it = book.begin(); it != book.end(); ++it)
                {
                    const Species &sp = *static_cast<const Species *>(*it);
                    Coerce(spfmt).updateWith(sp);
                    Coerce(species) << sp;
                }

            }

            //------------------------------------------------------------------
            //
            // assign SubLevel to ordered species
            //
            //------------------------------------------------------------------
            LightSort::MakeSubLevel<SList>( Coerce(species) );

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
            //
            // create topology
            //
            //------------------------------------------------------------------
            assert(size>0);
            assert(species.size>0);
            const size_t n = size;
            const size_t m = species.size;
            Coerce(Nu).make(n,m);
            for(const ENode *en=head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                eq.fill(Coerce(Nu)[eq.indx[SubLevel]],SubLevel);
            }
            Y_XMLOG(xml,"Nu   = " << Nu);

            if( n != MKL::Rank::Of(Nu) )
                throw Specific::Exception(here, "invalid system rank!!");

            //------------------------------------------------------------------
            //
            // create tables
            //
            //------------------------------------------------------------------
            for(const SNode *sn=species.head;sn;sn=sn->next)
            {
                if( ! Coerce(spset).record(**sn) ) throw Specific::Exception(here,"unexpected species multiple sub-index!");
            }

            for(const ENode *en=head;en;en=en->next)
            {
                if( ! Coerce(eqset).record(**en) ) throw Specific::Exception(here,"unexpected equilibirum multiple sub-index!");
            }




            buildCombinatorics(eqs,topK,xml);


        }
    }

}

