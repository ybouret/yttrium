
#include "y/chemical/weasel.hpp"
#include "y/lingo/syntax/xlist.hpp"
#include <iomanip>

namespace Yttrium
{
    namespace Chemical
    {

        Formula:: ~Formula() noexcept
        {

        }


        static inline
        void _build(Formula &formula)
        {
            static Weasel &weasel = Weasel::Instance();
            weasel.buildFormula(formula);

            std::cerr << "name ='" << formula.name  << "'" << std::endl;
            std::cerr << "   z ="  << formula.z     << std::endl;
        }

        Formula:: Formula(const XTree &code)   :
        name(),
        tree(code),
        z(0)
        {
            _build(*this);
        }

        

        Formula:: Formula(const String &expr) :
        name(),
        tree( Weasel::Instance().parseFormula(expr) ),
        z(0)
        {
            _build(*this);
        }


        Formula:: Formula(const char * const expr) :
        name(),
        tree( Weasel::Instance().parseFormula(expr) ),
        z(0)
        {
            _build(*this);
        }


        


    }

}


