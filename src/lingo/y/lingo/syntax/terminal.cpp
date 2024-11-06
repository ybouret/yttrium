
#include "y/lingo/syntax/terminal.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            Terminal:: ~Terminal() noexcept {}

            void Terminal:: viz(OutputStream &fp) const
            {
                Node(fp,this) << '[';
                Label(fp,*name);
                fp << "shape=box,style=\"filled";
                switch(kind)
                {
                    case Univocal: fp << ",bold"; break;
                    case Standard: break;
                }

                switch(role)
                {
                    case Semantic: break;
                    case Dividing: fp << ",dashed"; break;
                }
                Endl(fp << '\"' << ']');
            }
        }
    }
}

