
#include "y/apex/api/ortho/coven/survey.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {

                Survey:: ~Survey() noexcept
                {
                }

                Survey:: Survey() :
                proc(this, & Survey::check)
                {
                }

                void Survey:: check(const IList &l, const Vector &v)
                {
                    study(l,v);
                }
            }
        }
    }
}

