
#include "y/lingo/pattern/logic/or.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Or:: Or() noexcept :
        Logic(UUID)
        {
            Y_Lingo_Pattern(Or);
        }

        Or:: ~Or() noexcept {}

        Or:: Or(const Or &other) : Logic(other)
        {
            Y_Lingo_Pattern(Or);
        }

        Pattern * Or:: clone() const { return new Or(*this); }

        bool Or:: strong() const noexcept
        {
            return false;
        }

        bool Or:: univocal() const noexcept
        {
            return false;
        }

        void Or:: query(CharDB &fc) const
        {

        }

        String Or:: regularExpression() const
        {
            String ans;
            switch(size)
            {
                case 0: return ans;
                case 1: return head->regularExpression();
                default:
                    break;
            }
            ans += '(';
            const Pattern *node = head;
            ans += node->regularExpression();
            for(node=node->next;node;node=node->next)
                ans += '|' + node->regularExpression();
            ans += ')';
            return ans;
        }

        bool Or:: takes(Token &token, Source &source) const
        {
            assert(0==token.size);

            return false;
        }


        void Or:: viz(OutputStream &fp) const
        {
            for(const Pattern *node=head;node;node=node->next)
            {
                node->viz(fp);
            }

            Node(fp,this) << '[';
            Label(fp, "|") << ",shape=egg";
            Endl(fp << ']');

            vizLink(fp);
        }

    }

}


