
#include "y/mkl/fit/variable/primary.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Fit
        {
            PrimaryVariable:: ~PrimaryVariable() noexcept
            {
                Coerce(indx) = 0;
            }

            size_t PrimaryVariable:: CheckIndex(const String &name, const size_t j)
            {
                if(j<=0) throw Specific::Exception("Fit::PrimaryVariable", "'%s' has index 0!!'", name.c_str());
                return j;
            }

            size_t PrimaryVariable:: idx() const noexcept
            {
                return indx;
            }

            void PrimaryVariable:: dsp(std::ostream &os) const
            {
                os << '@' << indx;
            }
        }

    }

}
