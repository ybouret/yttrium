
#include "y/lingo/syntax/internal/compound/aggregate.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            Aggregate:: ~Aggregate() noexcept
            {
            }
            
            Aggregate & Aggregate:: operator += (const Rule &rule)
            {
                add(rule);
                return *this;
            }

            void Aggregate:: viz(OutputStream &fp) const
            {
                Node(fp,this) << "[";
                Label(fp, *name);
                Endl(fp << "]");
                vizLink(fp);
            }

            bool Aggregate:: robust() const noexcept
            {

            }

            bool Aggregate:: accepts(Y_Lingo_Syntax_Args) const
            {
                
            }
        }

    }

}
