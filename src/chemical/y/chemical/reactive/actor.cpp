#include "y/chemical/reactive/actor.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        Actor:: Actor(const unsigned n, const Species &s, const size_t i) :
        Entity(s.name,i),
        nu(n),
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

        Actor:: Actor(const Actor &a) :
        Entity(CopyOf,a),
        nu(a.nu),
        sp(a.sp),
        n1(a.n1),
        next(0),
        prev(0)
        {
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
    }

}
