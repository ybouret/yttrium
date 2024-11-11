
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

            bool Option:: robust() const   { return false; }

            const char * Option:: vizShape() const noexcept
            {
                return "diamond";
            }

            void Option:: viz(OutputStream &fp) const
            {
                Node(fp,this) << "[";
                Label(fp,*name);
                emitShape(fp);
                Endl(fp << "]");
                Endl(Arrow(fp,this, &rule));
            }


            bool Option:: accepts(Y_Lingo_Syntax_Args) const
            {
                // pass directly arguments to rule
                (void) rule.accepts(lexer,source,tree);
                return true;
            }

            Caption Option:: NameFor(const Rule &r)
            {
                const String s = *r.name + Suffix;
                return Caption(s);
            }

            Option:: Option(const Rule & _rule) :
            Wildcard( NameFor(_rule), UUID, _rule)
            {
                Y_Lingo_Syntax_Rule(Option);
            }

        }

    }

}
