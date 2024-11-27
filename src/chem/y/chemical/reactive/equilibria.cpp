
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
                db.print(os << '\t' << '@', **it) << std::endl;
            }
            os << '}';
            return os;
        }


        void Equilibria:: update() noexcept
        {
            reactor.update();
        }


    }

}

