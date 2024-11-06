
#include "y/lingo/syntax/internal/wildcard/option.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            Option:: ~Option() noexcept
            {
            }

            bool Option:: robust() const noexcept { return false; }

            void Option:: viz(OutputStream &fp) const
            {
                Node(fp,this) << "[";
                Label(fp,*name);
                fp << ",shape=diamond";
                Endl(fp << "]");
                Endl(Arrow(fp,this, &rule));
            }


            bool Option:: accepts(Y_Lingo_Syntax_Args) const
            {
                (void) rule.accepts(lexer,source,tree);
                return true;
            }

            Caption Option:: NameFor(const Rule &r)
            {
                const String s = *r.name + Suffix;
                return Caption(s);
            }
        }

    }

}
