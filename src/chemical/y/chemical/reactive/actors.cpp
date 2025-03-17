
#include "y/chemical/reactive/actors.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Actors:: CallSign = "Actors";


        Actors:: ~Actors() noexcept
        {
        }

        Actors:: Actors(const Actor::Involvement how) :
        Entity( new String() ),
        in(how),
        my()
        {
        }

        


        Y_PROXY_IMPL(Actors,my)

        bool Actors:: has(const Species &sp) const noexcept
        {
            for(const Actor *a=my.head;a;a=a->next)
            {
                if( & (a->sp) == &sp ) return true;
            }
            return false;
        }

        const Actor & Actors:: operator()(const unsigned nu, const Species &sp)
        {
            if(latched) throw Specific::Exception(CallSign,"latched while adding '%s'", sp.name->c_str());
            if(has(sp)) throw Specific::Exception(CallSign,"multiple species '%s'", sp.name->c_str());
            Actor * const actor = my.pushTail( new Actor(nu,sp,in) );

            try {
                String id = *name;
                if(my.size>1)
                {
                    switch(in)
                    {
                        case Actor::AsComponentOnly: id += " + "; break;
                        case Actor::AsConcentration: id += '+';   break;
                    }
                }
                id += *actor->name;
                Coerce(*name).swapWith(id);
            }
            catch(...)
            {
                delete my.pop(actor);
                throw;
            }

            return *actor;
        }


        void Actors:: Exchange(Actors &lhs, Actors &rhs)  noexcept
        {
            assert(lhs.in==rhs.in);
            Coerce(*lhs.name).swapWith(Coerce(*rhs.name));
            lhs.my.swapWith(rhs.my);
        }

    }

}
