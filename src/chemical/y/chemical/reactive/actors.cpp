
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

        bool Actors:: hired(const Species &sp) const noexcept
        {
            for(const Actor *mine=actors.head;mine;mine=mine->next)
            {
                if( &sp == & mine->sp) return true;
            }
            return false;
        }


        bool Actors:: hiredSome(const SList &sp) const noexcept
        {
            for(const SNode *sn=sp.head;sn;sn=sn->next)
            {
                if(hired(**sn)) return true;
            }
            return false;
        }

        bool Actors:: hiredFull(const SList &sp) const noexcept
        {
            for(const SNode *sn=sp.head;sn;sn=sn->next)
            {
                if( !hired(**sn)) return false;
            }
            return true;
        }



        bool Actors:: akin(const Actors &other) const noexcept
        {
            if(other.actors.size!=actors.size) return false;

            for(const Actor *mine = actors.head;mine;mine=mine->next)
            {
                if(!other.hired(mine->sp)) return false;
            }
            

            return true;
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
            if(hired(sp))
                throw Specific::Exception("Chemical::Actors", "multiple species '%s'", a->sp.name.c_str());


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
       
        void Actors:: massAction(XMul            &xmul, 
                                 const XReadable &C,
                                 const Level      L) const
        {
            for(const Actor *a=actors.head;a;a=a->next) 
                a->massAction(xmul,C,L);
        }

        void Actors:: massAction(XMul            &xmul,
                                 const XReadable &C,
                                 const Level      L,
                                 const xreal_t    xi
                                 ) const
        {
            for(const Actor *a=actors.head;a;a=a->next)
                a->massAction(xmul,C,L,xi);
        }

        bool Actors:: accounted(const XReadable &C, const Level L) const
        {
            const xreal_t zero;
            for(const Actor *a=actors.head;a;a=a->next)
            {
                if( C[a->sp.indx[L]] <= zero )
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


        void Actors:: moveSafe(XWritable &C, const Level L, const xreal_t xi) const
        {
            for(const Actor *a=actors.head;a;a=a->next) a->moveSafe(C,L,xi);
        }

        void Actors:: addSafe(XWritable       &Cout,
                              const Level      Lout,
                              const XReadable &Cinp,
                              const Level     &Linp,
                              const xreal_t    xi) const
        {
            for(const Actor *a=actors.head;a;a=a->next) a->addSafe(Cout, Lout, Cinp, Linp, xi);
        }

        void Actors:: drvsMassAction(XWritable       & drvs,
                                     const Level       Lout,
                                     const xreal_t     xfac,
                                     XMul            & xmul,
                                     const XReadable & Cinp,
                                     const Level       Linp) const
        {
            assert(xmul.isEmpty());
            for(const Actor *a=actors.head;a;a=a->next)
            {
                xmul.insert(xfac);                  // initialize xmul
                a->drvsMassAction(xmul,Cinp,Linp);  // store local
                for(const Actor *b=a->prev;b;b=b->prev) b->massAction(xmul,Cinp,Linp);
                for(const Actor *b=a->next;b;b=b->next) b->massAction(xmul,Cinp,Linp);
                drvs[ a->sp.indx[Lout] ] = xmul.product();
            }
        }

        void Actors:: drvsMassAction(XWritable       & drvs,
                                     XWritable       & Cout,
                                     const Level       Lout,
                                     const xreal_t     xfac,
                                     XMul            & xmul,
                                     const XReadable & Cinp,
                                     const Level       Linp,
                                     const xreal_t     xi) const
        {
            addSafe(Cout, Lout, Cinp, Linp, xi);
            drvsMassAction(drvs, Lout, xfac, xmul, Cout, Lout);
        }


        void Actors:: record(AddressBook &book) const
        {
            for(const Actor *a=actors.head;a;a=a->next)
                a->record(book);
        }

        int Actors:: charge() const noexcept
        {
            int z = 0;
            for(const Actor *a=actors.head;a;a=a->next)
                z += a->charge();
            return z;
        }

    }

}


