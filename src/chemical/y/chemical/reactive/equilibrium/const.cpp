
#include "y/chemical/reactive/equilibrium/const.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        ConstEquilibrium:: ConstEquilibrium(Library &     lib,
                                            XNode * const eNode,
                                            const size_t  ei) :
        RawEquilibrium(lib,eNode,ei),
        Konst( Eval(*Kdata) )
        {
        }


        ConstEquilibrium:: ~ConstEquilibrium() noexcept
        {
        }

        xreal_t ConstEquilibrium:: getK(xreal_t)
        {
            return Konst;
        }
    }

}

#include "y/chemical/weasel.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        xreal_t ConstEquilibrium:: Eval(const String &expr)
        {
            static Weasel &weasel = Weasel::Instance();
            std::cerr << "Evaluating '" << expr << "'" << std::endl;
            return weasel.eval(expr);
        }

    }
}

