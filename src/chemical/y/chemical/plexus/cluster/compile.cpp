

#include "y/chemical/plexus/cluster.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/algebra/rank.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        const Cluster & Cluster:: compile(XMLog      & xml,
                                          Equilibria & eqs,
                                          XWritable  & shK)
        {
            Y_XML_SECTION_OPT(xml, "Cluster", " eqs='" << size << "'");

            //------------------------------------------------------------------
            //
            // revamp equilibria
            //
            //------------------------------------------------------------------
            Indexed::SubOrganize(*this);
            Y_XMLOG(xml, "eqs  = " << *this);

            //------------------------------------------------------------------
            //
            // collect species
            //
            //------------------------------------------------------------------
            AddressBook book;
            {
                for(ENode *en=head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    update(eq);
                    eq.record(book);
                    if(eq->size()<=0) throw Specific::Exception(eq.name.c_str(),"no species!!");
                    if(!eq.neutral()) throw Specific::Exception(eq.name.c_str(),"not neutral!!");
                }
            }

            //------------------------------------------------------------------
            //
            // gather species
            //
            //------------------------------------------------------------------
            Indexed::SubOrganize(book.sendTo(Coerce(species)));
            Y_XMLOG(xml, "spc  = " << species);

            //------------------------------------------------------------------
            //
            // Build Nu
            //
            //------------------------------------------------------------------
            Coerce(Nu).make(size,species.size);
            for(const ENode *en=head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                MatrixRow<int>    &nu = Coerce(Nu[ eq.indx[SubLevel] ]);
                eq.topology(nu,SubLevel);
            }
            Y_XMLOG(xml, "Nu   = " << Nu);

            const size_t rank = MKL::Rank::Of(Nu);
            Y_XMLOG(xml, "rank = " << rank);
            if(rank!=size) throw Specific::Exception("Cluster","rank=%u < %u", unsigned(rank), unsigned(size));

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
            buildCombinatorics(xml,eqs,shK);

            return *this;
        }

    }

}


