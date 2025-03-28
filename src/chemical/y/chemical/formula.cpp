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
        Proxy<const XNode>(),
        code(_)
        {
            assert(code->defines<Formula>());
        }


        Formula:: Formula(XNode * const _) noexcept :
        Proxy<const XNode>(),
        code(_)
        {
            assert(code->defines<Formula>());
        }


        Formula:: Formula(const Formula &_) noexcept :
        Proxy<const XNode>(),
        code(_.code)
        {
            assert(code->defines<Formula>());
        }

       


        Y_PROXY_IMPL(Formula,*code)
    }

}
