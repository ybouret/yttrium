
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


            void Option:: vizMark(OutputStream &fp) const
            {
               fp << ",shape=diamond";
            }



            bool Option:: accepts(Y_Lingo_Syntax_Args) const
            {
                // pass directly arguments to rule
                Y_Lingo_Syntax_Rule_Emit(depth,"[Option '" << name << "']");
                const size_t ruleDepth = depth+1;
                (void) rule.accepts(lexer,source,tree,ruleDepth);
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
