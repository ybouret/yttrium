

#include "y/chemical/plexus/cluster.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/algebra/rank.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Cluster:: compile(XMLog &xml, Equilibria &primary)
        {
            EList &eqs = *this;
            Y_XML_SECTION_OPT(xml, "Cluster", " eqs='" << eqs.size << "'");

            //------------------------------------------------------------------
            //
            // revamp equilibria
            //
            //------------------------------------------------------------------
            Indexed::Organize(eqs);
            Y_XMLOG(xml, "eqs  = " << eqs);

            //------------------------------------------------------------------
            //
            // collect species
            //
            //------------------------------------------------------------------
            AddressBook book;
            {
                for(ENode *en=eqs.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    update(eq);
                    eq.record(book);
                    if(eq->size()<=0) throw Specific::Exception(eq.name.c_str(),"no species!!");
                }
            }

            //------------------------------------------------------------------
            //
            // gather species
            //
            //------------------------------------------------------------------
            {
                SList &mine = Coerce(species);
                for(AddressBook::Iterator it=book.begin();it!=book.end();++it)
                    mine << *static_cast<const Species *>(*it);
                Indexed::Organize(mine);
            }
            Y_XMLOG(xml, "spc  = " << species);

            //------------------------------------------------------------------
            //
            // Build Nu
            //
            //------------------------------------------------------------------
            Coerce(Nu).make(eqs.size,species.size);
            for(const ENode *en=eqs.head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                MatrixRow<int>    &nu = Coerce(Nu[ eq.indx[SubLevel] ]);
                eq.topology(nu,SubLevel);
            }
            Y_XMLOG(xml, "Nu   = " << Nu);

            const size_t rank = MKL::Rank::Of(Nu);
            Y_XMLOG(xml, "rank = " << rank);
            if(rank!=eqs.size) throw Specific::Exception("Cluster","rank=%u < %u", unsigned(rank), unsigned(eqs.size));

            //------------------------------------------------------------------
            //
            // Build Conservations
            //
            //------------------------------------------------------------------
            buildConservations(xml);
            Y_XMLOG(xml, "conserved=" << conserved.list);
            Y_XMLOG(xml, "unbounded=" << unbounded.list);

            //------------------------------------------------------------------
            //
            // Build Combinatoris
            //
            //------------------------------------------------------------------
            buildCombinatorics(xml,primary);

        }

    }

}


