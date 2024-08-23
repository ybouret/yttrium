
#include "y/chemical/reactive/component.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        Component:: ~Component() noexcept
        {
        }

        Component:: Component(const int n, const Species &s) :
        nu(n),
        sp(s),
        xn(nu)
        {
            if(0==nu) throw Specific::Exception("Component", "zero stoichiometry for '%s'", sp.name.c_str());
        }

        const String & Component:: key() const noexcept
        {
            return sp.key();;
        }

        std::ostream & operator<<(std::ostream &os, const Component &cm)
        {
            switch(cm.nu)
            {
                case 1:
                    break;
                case -1:
                    os << '-';
                    break;
                default:
                    os << cm.nu << '*';
                    break;
            }
            os << '[' << cm.sp << ']';
            return os;
        }
    }

}
