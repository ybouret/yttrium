
#include "y/chemical/reactive/actors.hpp"
#include "y/system/exception.hpp"
#include "y/ptr/auto.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Actors:: ~Actors() noexcept {}

        Actors:: Actors() :
        Entity(""),
        Proxy<const Actor::List>(),
        sumNu(0),
        scale(1),
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
            Coerce(sumNu) += a->nu;
            Coerce(scale)  = 1.0/(sumNu);

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

        bool Actors:: accounted(const XReadable &C0, const Level level) const
        {
            const xreal_t zero;
            for(const Actor *a=actors.head;a;a=a->next)
            {
                if( C0[a->sp.indx[level]] <= zero )
                {
                    return false;
                }
            }
            return true;
        }

        bool Actors:: deficient(const XReadable &C0, const Level level) const
        {
            const xreal_t zero = 0;
            for(const Actor *a=actors.head;a;a=a->next)
            {
                if( C0[a->sp.indx[level]] <= zero )
                {
                    return true;
                }
            }
            return false;
        }




        xreal_t Actors:: maxExtent(const XReadable &C, const Level level) const
        {
            const Actor *ac = actors.head; if(!ac) return 0;
            xreal_t      xi = ac->maxExtent(C,level);
            for(ac=ac->next;ac;ac=ac->next)
                xi = Min(xi,ac->maxExtent(C,level));
            return xi;
        }


        void Actors:: moveSafe(XWritable &C, const xreal_t xi, const Level level) const
        {
            for(const Actor *a=actors.head;a;a=a->next) a->moveSafe(C,xi,level);
        }

        void Actors:: drvsMassAction(XWritable       & phi,
                                     const Level       output,
                                     const xreal_t     xfac,
                                     XMul            & xmul,
                                     const XReadable & C,
                                     const Level       input) const
        {
            assert(xmul.isEmpty());
            for(const Actor *a=actors.head;a;a=a->next)
            {
                //const Species &sp = a->sp;
                xmul.insert(xfac);
                a->drvsMassAction(xmul,C,input);
                for(const Actor *b=a->prev;b;b=b->prev) b->massAction(xmul,C,input);
                for(const Actor *b=a->next;b;b=b->next) b->massAction(xmul,C,input);
                phi[ a->sp.indx[output] ] = xmul.product();
            }
        }


        void Actors:: record(AddressBook &book) const
        {
            for(const Actor *a=actors.head;a;a=a->next)
                a->record(book);
        }

    }

}


