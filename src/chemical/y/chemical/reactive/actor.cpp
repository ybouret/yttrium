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
                                const XReadable &C,
                                const Level      L,
                                const xreal_t    xi) const
        {
            const xreal_t zero;
            const xreal_t c = Max(zero,C[sp.indx[L]] + xn * xi);
            std::cerr << "{([" << sp.name << "]=" << c <<"])^" << nu << "}";
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
            else
            {
                assert(1==nu);
                // do nothing, derivative is one
            }
        }


        xreal_t Actor:: maxExtent(const XReadable &C, const Level L) const
        {
            const xreal_t c = C[sp.indx[L]]; assert(c>=xreal_t(0));
            return c/xn;
        }

        void Actor:: moveSafe(XWritable &C,const Level L, const xreal_t xi) const
        {
            const xreal_t zero;
            xreal_t      & c    = C[sp.indx[L]];
            const xreal_t  cnew = c + xn*xi;
            c = Max(zero,cnew);
        }

        void Actor:: addSafe(XWritable       &Cout,
                             const Level      Lout,
                             const XReadable &Cinp,
                             const Level     &Linp,
                             const xreal_t    xi) const
        {
            const xreal_t zero;
            Cout[sp.indx[Lout]] = Max(zero, Cinp[sp.indx[Linp]] + xn * xi);
        }

        void Actor:: record(AddressBook &book) const
        {
            book |= sp;
        }

        int Actor:: charge() const noexcept
        {
            return int(nu) * sp.z;
        }

        void Actor:: viz(OutputStream &             fp,
                         const Entity &             peer,
                         const String &             color,
                         const Iterating::Direction dir) const
        {
            const Vizible *source = &sp;
            const Vizible *target = &peer;
            if(dir==Iterating::Reverse) Swap(source,target);
            Arrow(fp,source,target) << '[';
            fp << "color=" << color << ",fontcolor=" << color;
            if(nu>1) {
                const String label = Formatted::Get("%u",nu);
                Label(fp << ",",label);
            }
            fp << ']';
            Endl(fp);
        }

    }

}
