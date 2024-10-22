

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

        Pattern * None:: clone() const
        {
            return new None(*this);
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
            
            vizLink(fp);
        }

        bool None:: strong() const noexcept
        {
            return false;
        }

        bool None:: univocal() const noexcept
        {
            return false;
        }

        void None:: query(CharDB &fc) const
        {

        }

        bool None:: takes(Token &token, Source &source) const
        {
            assert(0==token.size);

            return false;
        }

        String None:: regularExpression() const
        {
            String ans = "[^";
            for(const Pattern *node=head;node;node=node->next)
            {
                ans += node->regularExpression();
            }
            return ans + "]";
        }

        
    }

}


