

#include "y/lingo/pattern/logic/none.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        None:: None() noexcept :
        Logic(UUID)
        {
            Y_Lingo_Pattern(None);
        }

        None:: ~None() noexcept {}

        None::None(const None &other) :
        Logic(other)
        {
            Y_Lingo_Pattern(None);
        }



        void None:: viz(OutputStream &fp) const
        {
            for(const Pattern *node=head;node;node=node->next)
            {
                node->viz(fp);
            }

            Node(fp,this) << '[';
            Label(fp, "^") << ",shape=diamond";
            Endl(fp << ']');

            vizLink(fp,*this);
        }
    }

}


