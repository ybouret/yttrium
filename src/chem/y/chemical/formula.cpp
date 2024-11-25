
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

            std::cerr << "title='" << formula.title << "'" << std::endl;
            std::cerr << "    z="  << formula.z     << std::endl;
        }

        Formula:: Formula(const XCode &code)   :
        title( new String() ),
        xcode( code  ),
        z(0)
        {
            _build(*this);
        }

        

        Formula:: Formula(const String &expr) :
        title( new String() ),
        xcode( Weasel::Instance().parseFormula(expr) ),
        z(0)
        {
            _build(*this);
        }


        Formula:: Formula(const char * const expr) :
        title( new String() ),
        xcode( Weasel::Instance().parseFormula(expr) ),
        z(0)
        {
            _build(*this);
        }


        Formula:: Formula(const Formula &_) noexcept :
        title( _.title ),
        xcode( _.xcode ),
        z(     _.z     )
        {

        }


    }

}


