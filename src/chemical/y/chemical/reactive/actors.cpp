
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
        my(),
        sum(0),
        kxp(1)
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
            
            Coerce(sum) += nu;
            Coerce(kxp)  = 1.0 / sum;
            return *actor;
        }
        
        
        void Actors:: Exchange(Actors &lhs, Actors &rhs)  noexcept
        {
            assert(lhs.in==rhs.in);
            Coerce(*lhs.name).swapWith(Coerce(*rhs.name));
            lhs.my.swapWith(rhs.my);
        }
        
        
        void Actors:: activity(XMul &X, const XReadable &C, const Level L) const
        {
            for(const Actor *a=my.head;a;a=a->next) a->activity(X,C,L);
        }
        
        
        void Actors:: activity(XMul &X, const XReadable &C, const Level L, const xreal_t xi) const
        {
            for(const Actor *a=my.head;a;a=a->next) a->activity(X,C,L,xi);
        }
        
        
        bool Actors:: critical(const XReadable &C, const Level L) const noexcept
        {
            for(const Actor *a=my.head;a;a=a->next)
            {
                if( a->sp(C,L).mantissa <= 0 ) return true;
            }
            return false;
        }
        
        xreal_t Actors:: limiting(const XReadable &C, const Level L) const noexcept
        {
            assert(my.size>0);
            const Actor *ac = my.head;
            xreal_t      xi = ac->limiting(C,L);
            for(ac=ac->next;ac;ac=ac->next)
                InSituMin(xi,ac->limiting(C,L) );
            return xi;
        }
        
        
        
        void Actors:: safeMove(XWritable &C, const Level L, const xreal_t xi) const noexcept
        {
            for(const Actor *a=my.head;a;a=a->next) a->safeMove(C,L,xi);
        }

        void Actors:: boldMove(XWritable &C, const Level L, const xreal_t xi) const noexcept
        {
            for(const Actor *a=my.head;a;a=a->next) a->boldMove(C,L,xi);
        }


        static inline
        void showActor(std::ostream &      os,
                       const Actor * const a,
                       const XReadable &   C,
                       const Level         L)
        {
            assert(0!=a);
            const Species &sp = a->sp;
            os << '[' << sp.name << ']' << '=' << sp(C,L).str();
        }

        std::ostream & Actors:: displayCompact(std::ostream &  os,
                                     const XReadable &C,
                                     const Level      L) const
        {
            os << '{';
            const Actor *a=my.head;
            if(a)
            {
                showActor(os,a,C,L);
                for(a=a->next;a;a=a->next)
                    showActor(os << ';',a,C,L);
            }
            os << '}';
            return os;
        }


        void Actors:: sendTo(AddressBook &book) const
        {
            for(const Actor *a=my.head;a;a=a->next)
                book |= a->sp;
        }


        void Actors:: drvsActivity(XWritable &      phi,
                                   const Level      lvl,
                                   const xreal_t    fac,
                                   XMul            &X,
                                   const XReadable &C,
                                   const Level      L) const
        {
            for(const Actor *a=my.head;a;a=a->next)
            {
                X.free();
                X << fac;
                a->drvsActivity(X,C,L);
                for(const Actor *b=a->prev;b;b=b->prev) b->activity(X,C,L);
                for(const Actor *b=a->next;b;b=b->next) b->activity(X,C,L);
                a->sp(phi,lvl) = X.product();
            }
        }


    }

}
