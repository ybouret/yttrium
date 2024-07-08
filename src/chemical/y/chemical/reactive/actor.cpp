#include "y/chemical/reactive/actor.hpp"
#include "y/system/exception.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        Actor:: Actor(const unsigned n, const Species &s) :
        Entity(s.name),
        nu(n),
        xn(nu),
        sp(s),
        n1(nu-1),
        next(0),
        prev(0)
        {
            const char * const id = s.name.c_str();
            switch(nu)
            {
                case 0: throw Specific::Exception("Chemical::Actor","zero stoichiometry for '%s'", s.name.c_str());
                case 1:  {  String _ = Formatted::Get("[%s]",id);       Coerce(name).swapWith(_); } break;
                default: {  String _ = Formatted::Get("%u*[%s]",nu,id); Coerce(name).swapWith(_); } break;
            }

        }




        Actor:: ~Actor() noexcept
        {
            
        }

        std::ostream & operator<<(std::ostream &os, const Actor &ac)
        {
            return os<<ac.name;
        }

        const Actor & Actor:: operator*() const noexcept
        {
            return *this;
        }

        void Actor:: massAction(XMul &xmul, const XReadable &C, const Level level) const
        {
            const xreal_t c = C[sp.indx[level]]; assert(c>=xreal_t(0));
            xmul.insert(c,nu);
        }

        void Actor:: massAction(XMul            &xmul,
                                const XReadable &C0,
                                const xreal_t    xi,
                                const Level      level) const
        {
            const xreal_t zero = 0;
            const xreal_t c = Max(zero,C0[sp.indx[level]] + xn * xi);
            xmul.insert(c,nu);
        }


        void Actor:: drvsMassAction(XMul            &xmul,
                                    const XReadable &C,
                                    const Level      level) const
        {
            if(nu>1)
            {
                const xreal_t c = C[sp.indx[level]]; assert(c>=xreal_t(0));
                xmul.insert(c,n1);
                xmul.insert(xn);
            }
        }


    }

}
