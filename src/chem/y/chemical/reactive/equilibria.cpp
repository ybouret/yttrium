
#include "y/chemical/reactive/equilibria.hpp"
#include "y/system/exception.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        Reactor:: ~Reactor() noexcept {}
        Reactor:: Reactor() :
        Equilibrium::Set(),
        Fragment()
        {
        }

        void Reactor:: mustInsert(const Equilibrium::Handle &handle)
        {
            if( !insert(handle) ) throw Specific::Exception( Equilibria::CallSign, "multiple '%s'", handle->name.c_str());
            enroll( *handle );
        }

        void Reactor:: update() noexcept
        {
            for(Iterator it=begin();it!=end();++it)
                enroll(**it);
        }

    }

}


namespace Yttrium
{
    namespace Chemical
    {

        const char * const Equilibria:: CallSign = "Chemical::Equilibria";

        Equilibria:: Equilibria() : Proxy<const Reactor>(), reactor()
        {
        }

        Equilibria:: ~Equilibria() noexcept
        {
        }

        Equilibria:: ConstInterface & Equilibria:: surrogate() const noexcept
        {
            return reactor;
        }

        void Equilibria:: decl( Equilibrium * const eq)
        {
            assert(0!=eq);
            const Equilibrium::Handle handle(eq);
            reactor.mustInsert(eq);
        }


        std::ostream & operator<<(std::ostream &os, const Equilibria &eqs)
        {
            const Reactor &db = eqs.reactor;
            if(db.size()<=0)
                return os << "{}";
            os << '{' << std::endl;
            for(Equilibria::ConstIterator it=eqs->begin();it!=eqs->end();++it)
            {
                const Equilibrium &eq = **it;
                db.print(os << '\t' << '@', eq);
                const xReal  K = eq.K(0);
                const real_t l = K.log10();
                os << "'10^(" << l << ")'";
                os << std::endl;
            }
            os << '}';
            return os;
        }


        void Equilibria:: updateFragment() noexcept
        {
            reactor.update();
        }


        void Equilibria:: graphViz(OutputStream &fp) const
        {
            GraphViz::Vizible::Enter(fp, "G");
            {
                SList       species;
                {
                    AddressBook book;
                    for(ConstIterator it=reactor.begin();it!=reactor.end();++it)
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

            for(ConstIterator it=reactor.begin();it!=reactor.end();++it)
            {
                const Components &cm = **it;
                const String      color = cm.makeColor();
                cm.viz(fp,color.c_str(),0);
            }

            GraphViz::Vizible::Leave(fp);

        }



    }

}

