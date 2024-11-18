
#include "y/lingo/syntax/grammar.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            static inline
            XNode * reloadTerminal(const Terminal &rule,
                                   InputStream    &fp)
            {
                
            }


            XNode * Grammar:: reload(InputStream &fp) const
            {
                const Grammar &self     = *this;
                const String   ruleName = String::ReadFrom(fp);
                const Rule    &rule     = self[ruleName];
                if(rule.isTerminal()) {
                    return reloadTerminal( *rule.as<Terminal>(), fp);
                }
                else
                {

                }
            }
        }

    }

}

