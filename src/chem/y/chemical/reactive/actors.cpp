#include "y/chemical/reactive/actors.hpp"



namespace Yttrium
{
    namespace Chemical
    {
        Actors:: Actors(const NameForm disp) noexcept : Proxy<const Company>(), company(disp)
        {
        }

        Actors:: Actors(const Actors &_)  : Proxy<const Company>(), company(_.company)
        {
        }

        Actors:: ~Actors() noexcept
        {
        }


        Actors::ConstInterface & Actors::surrogate() const noexcept { return company; }

        const Actor & Actors:: operator()(const Species &sp)
        {
            assert(!company.lookFor(sp));
            return company.recruit( new Actor(sp) );
        }

        const Actor & Actors:: operator()(const unsigned nu, const Species &sp)
        {
            assert(!company.lookFor(sp));
            assert(nu>0);
            return company.recruit( new Actor(nu,sp) );
        }

        std::ostream &   operator<<(std::ostream &os, const Actors &actors)
        {
            return os << (actors->name);
        }

        void Actors:: activity(XMul &xmul, const XReadable &C, const Level L) const
        {
            for(const Actor *a=company.head;a;a=a->next) a->activity(xmul,C,L);
        }

        void Actors:: activity(XMul &xmul, const XReadable &C, const Level L, const xReal xi) const
        {
            for(const Actor *a=company.head;a;a=a->next) a->activity(xmul,C,L,xi);
        }



        bool Actors:: haveZero(const XReadable &C, const Level L) const noexcept
        {
            for(const Actor *a=company.head;a;a=a->next)
            {
                const xReal c = a->sp(C,L);
                switch( Sign::Of(c.mantissa) )
                {
                    case Negative:
                    case __Zero__:
                        return true;
                    case Positive:
                        continue;
                }
            }
            return false;
        }

        xReal Actors:: limitingExtent(const XReadable &C, const Level L) const noexcept
        {
            if(company.size<=0)
                return 0;
            else
            {
                const Actor *a  = company.head;
                xReal        xi = a->sp(C,L)/a->xn; assert(xi>=0.0);
                for(a=a->next;a;a=a->next)
                {
                    const xReal xt = a->sp(C,L)/a->xn; assert(xt>=0.0);
                    if(xt<xi) xi=xt;
                }
                return xi;
            }

        }

        void  Actors:: moveSafely(XWritable &C, const Level L, const xReal xi) const noexcept
        {
            for(const Actor *a=company.head;a;a=a->next)
                a->moveSafely(C,L,xi);

        }



    }

}
