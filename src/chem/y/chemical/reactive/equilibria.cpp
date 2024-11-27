
#include "y/chemical/reactive/equilibria.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Reactor:: ~Reactor() noexcept {}
        Reactor:: Reactor() :
        Equilibrium::Set(),
        Assembly()
        {
        }

        void Reactor:: mustInsert(const Equilibrium::Handle &handle)
        {
            if( !insert(handle) ) throw Specific::Exception( Equilibria::CallSign, "multiple '%s'", handle->name.c_str());
            enroll( *handle );
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

    }

}

