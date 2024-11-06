
#include "y/lingo/syntax/rule.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            Rule:: ~Rule() noexcept
            {
            }


            void Rule:: graphViz(OutputStream &fp) const
            {
                Enter(fp, "G");
                viz(fp);
                Leave(fp);
            }

            bool Rule:: flimsy() const noexcept
            {
                return !robust();
            }

        }

    }
}



namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            XNode:: XNode(const Rule   & _rule, Lexeme * const _unit) noexcept:
            Object(),
            rule(_rule),
            type(Terminal),
            sire(0),
            next(0),
            prev(0)
            {
                assert(0!=_unit);
                assert(Terminal==rule.typeOfNode());
                zero();
                unit = _unit;
            }

            XNode:: XNode(const Rule & _rule) noexcept:
            Object(),
            rule(_rule),
            type(Internal),
            sire(0),
            next(0),
            prev(0)
            {
                assert(Internal==rule.typeOfNode());
                zero();
                new( & list() ) XList();
            }
        }

    }

}

