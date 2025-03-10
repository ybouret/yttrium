#include "y/chemical/formula.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        const char * const Formula:: CallSign = "Formula";
        const char * const Formula:: Name     = "Name";
        const char * const Formula:: Body     = "Body";
        const char * const Formula:: Mult     = "Mult";
        const char * const Formula:: Z        = "Z";


        Formula:: ~Formula() noexcept
        {

        }

        Formula:: Formula(const XCode &_) noexcept :
        code(_)
        {
        }


        Formula:: Formula(const XNode * const _) noexcept :
        code(_)
        {
        }


        Formula:: Formula(const Formula &_) noexcept :
        code(_.code)
        {
            
        }

       


        Y_PROXY_IMPL(Formula,*code)
    }

}
