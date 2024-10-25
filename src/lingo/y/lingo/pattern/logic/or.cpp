
#include "y/lingo/pattern/logic/or.hpp"
#include "y/lingo/pattern/char-db.hpp"

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
            if(size<=0)
                // any1
                fc.fill();
            else
                // all possible fc
                for(const Pattern *p=head;p;p=p->next)
                    p->query(fc);

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

            if(size<=0)
                return source.getch(token);
            else
            {
                Token local;
                bool  found = false;
                for(const Pattern *p=head;p;p=p->next)
                {
                    if(p->takes(local,source))
                    {
                        found = true;
                        if(local.size<=0)
                        {
                            // feeble pattern, give it another try if possible
                            continue;
                        }
                        else
                        {
                            // not empty => success
                            token << local;
                            break;
                        }
                    }
                }

                return found;
            }
        }


        void Or:: viz(OutputStream &fp) const
        {
            for(const Pattern *node=head;node;node=node->next)
                node->viz(fp);


            Node(fp,this) << '[';
            Label(fp, "|") << ",shape=egg";
            Endl(fp << ']');

            vizLink(fp);
        }

    }

}


