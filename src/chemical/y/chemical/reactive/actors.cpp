
#include "y/chemical/reactive/actors.hpp"
#include "y/system/exception.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Actors:: ~Actors() noexcept {}

        Actors:: Actors() :
        Entity(""),
        Proxy<const Actor::List>(),
        actors() {}


        


        Actors::ConstInterface &Actors:: surrogate() const noexcept { return actors; }

        std::ostream & operator<<(std::ostream &os, const Actors &ac)
        {
            return os<<ac.name;
        }
       
        void Actors:: growName(const String &s)
        {
            String _ = name + s;
            Coerce(name).swapWith(_);
        }


        void Actors:: operator()(const unsigned nu, const Species &sp)
        {
            //__________________________________________________________________
            //
            // reserve actor
            //__________________________________________________________________
            Actor         *a = new Actor(nu,sp);
            AutoPtr<Actor> guard(a);
           
            //__________________________________________________________________
            //
            // check no multiple species
            //__________________________________________________________________
            for(const Actor *mine=actors.head;mine;mine=mine->next)
            {
                if( & (mine->sp) == &(a->sp) )
                    throw Specific::Exception("Chemical::Actors", "multiple species '%s'", a->sp.name.c_str());
            }

            //__________________________________________________________________
            //
            // change name
            //__________________________________________________________________
            if(actors.size<=0)
                growName(a->name);
            else
            {
                const String ext = '+' + a->name;
                growName(ext);
            }

            //__________________________________________________________________
            //
            // store new actor
            //__________________________________________________________________
            actors.pushTail( guard.yield() );

        }
       
        void Actors:: massAction(XMul &xmul, const XReadable &C, const Level level) const
        {
            for(const Actor *a=actors.head;a;a=a->next) a->massAction(xmul,C,level);
        }

        void Actors:: massAction(XMul            &xmul,
                                 const XReadable &C0,
                                 const xreal_t    xi,
                                 const Level      level) const
        {
            for(const Actor *a=actors.head;a;a=a->next) a->massAction(xmul,C0,xi,level);
        }

        bool Actors:: wouldRun(const XReadable &C0, const Level level) const
        {
            const xreal_t zero = 0;
            for(const Actor *a=actors.head;a;a=a->next)
            {
                if( C0[a->sp.indx[level]] <= zero )
                {
                    return false;
                }
            }
            return true;
        }

    }

}


