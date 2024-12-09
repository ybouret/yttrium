
#include "y/chemical/reactive/equilibria.hpp"
#include "y/system/exception.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        Reactor:: ~Reactor() noexcept {}
        Reactor:: Reactor() :
        Equilibrium::Set(),
        Fragment(),
        K()
        {
        }

        void Reactor:: mustInsert(const Equilibrium::Handle &handle)
        {
            // check index
            const size_t nextIndex = size() + 1;
            if(nextIndex != handle->indx[TopLevel])
                throw Specific::Exception( Equilibria::CallSign,"invalid index for '%s'", handle->name.c_str());

            // syncrhonize constants
            {
                const xReal _0;
                K.adjust( nextIndex, _0);
            }

            try
            {
                if( !insert(handle) )
                    throw Specific::Exception( Equilibria::CallSign, "multiple '%s'", handle->name.c_str());
                enroll( *handle );
            }
            catch(...)
            {
                K.popTail();
                assert( size() == K.size() );
                throw;
            }

            assert( size() == K.size() );
            K[ size() ] = handle->K(0);
        }



    }

}


namespace Yttrium
{
    namespace Chemical
    {

        const char * const Equilibria:: CallSign = "Chemical::Equilibria";

        Equilibria:: Equilibria() :
        Proxy<const Reactor>(),
        reactor(),
        K( reactor.K )
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
                const xReal  K = eqs.K[ eq.indx[TopLevel] ];
                const real_t l = K.log10();
                os << "'10^(" << l << ")'";
                os << std::endl;
            }
            os << '}';
            return os;
        }


        void Equilibria:: updateFragment() noexcept
        {
            reactor.updateWith(reactor.begin(), reactor.size() );
        }


        XReadable & Equilibria:: updateK(xReal t) const
        {
            ConstIterator it = reactor.begin();
            for(size_t i=reactor.size();i>0;--i)
            {
                const Equilibrium &eq = **it;
                const Indexed     &id = eq;
                id(K,TopLevel) = eq.K(t);
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

