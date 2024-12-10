
#include "y/chemical/reactive/equilibria.hpp"
#include "y/system/exception.hpp"



namespace Yttrium
{
    namespace Chemical
    {


        Equilibria:: Equilibria() :
        Proxy<const Reactor>(),
        my(),
        K( my.K )
        {
        }

        Equilibria:: ~Equilibria() noexcept
        {
        }

        Equilibria:: ConstInterface & Equilibria:: surrogate() const noexcept
        {
            return my;
        }

        void Equilibria:: decl( Equilibrium * const eq)
        {
            assert(0!=eq);
            const Equilibrium::Handle handle(eq);
            my.mustInsert(eq);
        }


        std::ostream & operator<<(std::ostream &os, const Equilibria &eqs)
        {
            const Reactor &db = eqs.my;
            if(db.size()<=0)
                return os << "{}";
            os << '{' << std::endl;
            for(Equilibria::ConstIterator it=eqs->begin();it!=eqs->end();++it)
            {
                const Equilibrium &eq = **it;
                db.print(os << '\t' << '@', eq);
                const xReal  K = eq(eqs.K,TopLevel);//eqs.K[ eq.indx[TopLevel] ];
                const real_t l = K.log10();
                os << "'10^(" << l << ")'";
                os << std::endl;
            }
            os << '}';
            return os;
        }


        void Equilibria:: updateFragment() noexcept
        {
            my.updateWith(my.begin(), my.size() );
        }


        XReadable & Equilibria:: updateK(xReal t) const
        {
            ConstIterator it = my.begin();
            for(size_t i=my.size();i>0;--i)
            {
                const Equilibrium &eq = **it;
                eq(K,TopLevel) = eq.K(t);
             }
            return K;
        }


        void Equilibria:: graphViz(OutputStream &fp) const
        {
            GraphViz::Vizible::Enter(fp, "G");
            {
                SList       species;
                {
                    AddressBook book;
                    for(ConstIterator it=my.begin();it!=my.end();++it)
                    {
                        const Components &cm = **it;
                        cm.addSpeciesTo(book);
                    }
                    DBOps::Extract(species,book);
                }

                for(const SNode *sn=species.head;sn;sn=sn->next)
                {
                    const Species &sp    = **sn;
                    const String   color = sp.makeColor();
                    sp.viz(fp,color.c_str(),0);
                }
            }

            for(ConstIterator it=my.begin();it!=my.end();++it)
            {
                const Components &cm = **it;
                const String      color = cm.makeColor();
                cm.viz(fp,color.c_str(),0);
            }

            GraphViz::Vizible::Leave(fp);

        }



    }

}

