
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
            forbidden0(StrongFn);
            for(const Pattern *p=head;p;p=p->next)
            {
                if(p->feeble()) return false;
            }
            return true;

        }

        bool Or:: univocal() const  
        {
            forbidden0(UnivocalFn);
            return 1==size && head->univocal();
        }

        void Or:: query(CharDB &fc) const
        {
            forbidden0(QueryFn);
            for(const Pattern *p=head;p;p=p->next)
                p->query(fc);
        }

        String Or:: regularExpression() const
        {
            forbidden0(RegExpFn);
           if(1==size)
               return head->regularExpression();
            else
            {
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
        }

        bool Or:: takes(Token &token, Source &source) const
        {
            assert(0==token.size);

            forbidden0(TakesFn);
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


