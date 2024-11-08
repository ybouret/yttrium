#include "y/lingo/syntax/internal/compound/alternate.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            Alternate:: ~Alternate() noexcept
            {
            }

            Alternate & Alternate:: operator |= (const Rule &rule)
            {
                add(rule);
                return *this;
            }



            void Alternate:: viz(OutputStream &fp) const
            {
                Node(fp,this) << "[";
                Label(fp, *name);
                fp << ",shape=egg";
                Endl(fp << "]");
                vizLink(fp);
            }

            bool Alternate:: robust() const
            {
                checkNotEmpty("Alternate::robust()",name->c_str());
                for(const RNode *node=manifest.head;node;node=node->next)
                {
                    if( (**node).flimsy() ) return false;
                }
                return false;
            }

            bool Alternate:: accepts(Y_Lingo_Syntax_Args) const
            {
                assert(manifest.size>0);
                
            }

        }

    }

}
