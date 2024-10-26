

#include "y/lingo/pattern/logic/none.hpp"
#include "y/lingo/pattern/char-db.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Y_Lingo_Pattern_CallSign(None)


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

        bool None:: _strong() const 
        {
            return false;
        }

        bool None:: _univocal() const
        {
            return false;
        }

        void None:: _query(CharDB &fc) const
        {
            assert(size>0);
            CharDB bad;
            for(const Pattern *p=head;p;p=p->next)
                p->query(bad);
            fc += bad.inv();
        }

        bool None:: _takes(Token &token, Source &source) const
        {
            assert(0==token.size);
            assert(size>0);
            
            return false;
        }

        String None:: _regexp() const
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


