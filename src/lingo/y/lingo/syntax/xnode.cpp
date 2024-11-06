
#include "y/lingo/syntax/xnode.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            XNode:: ~XNode() noexcept
            {
            }


            XNode * XNode:: CreateFrom(const Rule &rule, Lexeme * const unit)
            {
                assert(0!=unit);
                try {
                    return new XNode(rule,unit);
                }
                catch(...)
                {
                    delete unit;
                    throw;
                }
            }


            XNode * XNode:: CreateFrom(const Rule &rule)
            {
                return new XNode(rule);
            }

            

        }

    }

}


