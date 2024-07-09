
#include "y/chemical/plexus/cluster.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Cluster:: ~Cluster() noexcept
        {
        }

        Cluster:: Cluster(const Equilibrium &first) :
        Proxy<const EList>(),
        Party(),
        eqs(),
        species(),
        next(0),
        prev(0)
        {
            eqs << first;
        }

        Cluster:: ConstInterface & Cluster:: surrogate() const noexcept { return eqs; }


        bool Cluster:: accept(const Equilibrium &eq) const noexcept
        {
            for(const ENode *en=eqs.head;en;en=en->next)
            {
                if( eq.linkedTo( **en) ) return true;
            }
            return false;
        }

        bool Cluster:: accept(const Cluster &cl) const noexcept
        {
            for(const ENode *en=eqs.head;en;en=en->next)
            {
                if(cl.accept(**en)) return true;
            }
            return false;
        }

        void Cluster:: compile(XMLog &xml)
        {
            Y_XML_SECTION_OPT(xml, "Cluster", " eqs='" << eqs.size << "'");

            // sort equilibria
            Indexed::SortBy<TopLevel>::Using(eqs);

            // collect species
            AddressBook book;
            {
                size_t i=0;
                for(ENode *en=eqs.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    Coerce( eq.indx[SubLevel] ) = ++i;
                    update(eq);
                    eq.record(book);
                    if(eq->size()<=0) throw Specific::Exception(eq.name.c_str(),"no species!!");
                }
            }

            // gather species
            for(AddressBook::Iterator it=book.begin();it!=book.end();++it)
            {
                Coerce(species) << *static_cast<const Species *>(*it);
            }



            // label species
            Indexed::SortBy<TopLevel>::Using(Coerce(species));
            {
                size_t j=0;
                for(const SNode *sn=species.head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    Coerce( sp.indx[SubLevel] ) = ++j;
                }
            }




        }



    }

}


