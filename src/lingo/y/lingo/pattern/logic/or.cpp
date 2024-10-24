
#include "y/lingo/pattern/logic/or.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Y_Lingo_Pattern_CallSign(Or)


        Or:: Or() noexcept :
        Logic(UUID)
        {
            Y_Lingo_Pattern(Or);
        }

        Or:: ~Or() noexcept
        {
            std::cerr << "~Or" << std::endl;
        }

        Or:: Or(const Or &other) : Logic(other)
        {
            Y_Lingo_Pattern(Or);
        }

        Pattern * Or:: clone() const { return new Or(*this); }

        bool Or:: strong() const noexcept
        {
            if(size<=0)
            {
                return true;
            }
            else
            {
                for(const Pattern *p=head;p;p=p->next)
                {
                    if(p->feeble()) return false;
                }
                return true;
            }
        }

        bool Or:: univocal() const noexcept
        {
            return 1==size && head->univocal();
        }

        void Or:: query(CharDB &fc) const
        {
            for(const Pattern *p=head;p;p=p->next)
            {
                p->query(fc);
            }
        }

        String Or:: regularExpression() const
        {
            switch(size)
            {
                case 0: return "[]";
                case 1: return head->regularExpression();
                default:
                    break;
            }
            assert(size>=2);
            String ans;
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


