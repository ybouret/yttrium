
#include "y/chemical/reactive/equilibrium/actor.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        Actor:: ~Actor() noexcept
        {
        }


        static inline
        String actorPrefix(const unsigned nu)
        {
            return nu>1 ? Formatted::Get("%u",nu) : String();
        }

        static inline
        String actorName(const unsigned nu,
                         const Species &sp,
                         const NameForm nf)
        {
            String res = actorPrefix(nu);
            switch(nf)
            {
                case NameAsNormalSpecies: res += sp.name; break;
                case NameAsConcentration: res += '[' + sp.name + ']'; break;
            }
            return res;
        }

        Actor:: Actor(const unsigned _nu, const Species & _sp, const NameForm form) :
        Entity(),
        nu(_nu),
        sp(_sp),
        xn(nu),
        n1(nu-1),
        next(0),
        prev(0),
        name( actorName(nu,sp,form) )
        {
            assert(nu>0);
        }

        Actor:: Actor(const Species & _sp, const NameForm form) :
        Entity(),
        nu(1),
        sp(_sp),
        xn(1),
        n1(0),
        next(0),
        prev(0),
        name(  actorName(nu,sp,form) )
        {
        }

        const String & Actor:: key() const noexcept
        {
            return name;
        }

        Actor:: Actor(const Actor &_) :
        Entity(),
        nu(_.nu),
        sp(_.sp),
        xn(_.xn),
        n1(_.n1),
        next(0),
        prev(0),
        name(_.name)
        {
        }


        const Actor & Actor:: operator*() const noexcept { return *this; }


        void Actor::  moveSafely(XWritable &C, const Level L, const xReal xi) const noexcept
        {
            const xReal zero;
            xReal &c = sp(C,L);
            assert(c>=zero);
            c += xi * xn;
            if(c<=zero) c = zero;
        }


        void Actor:: activity(XMul &xmul, const XReadable &C, const Level L) const
        {
            assert(sp(C,L)>=0.0);
            xmul.insert( sp(C,L), nu);
        }

        void Actor:: activity(XMul &xmul, const XReadable &C, const Level L, const xReal xi) const
        {
            assert(sp(C,L)>=0.0);
            xReal c = sp(C,L) + xn * xi;
            if(c<=0.0) c=0.0;
            xmul.insert(c,nu);
        }

        String Actor:: html(const NameForm form) const
        {
            String res = actorPrefix(nu);
            switch(form)
            {
                case NameAsNormalSpecies: res += sp.html(); break;
                case NameAsConcentration: res += '[' + sp.html() + ']'; break;
            }
            return res;
        }

    }

}
