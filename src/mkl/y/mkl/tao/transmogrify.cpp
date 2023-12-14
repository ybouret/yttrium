
#include "y/mkl/tao/transmogrify.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Tao
        {
            namespace Cog
            {
                const char * TransmogrifyInfo::ToText(const Relationship r) noexcept
                {
                    switch(r)
                    {
                        case IsSubClassOf: return " is subclass of ";
                        case MustCastFrom: return " must cast from ";
                    }
                    return Core::Unknown;
                }
            }

        }

    }
}
