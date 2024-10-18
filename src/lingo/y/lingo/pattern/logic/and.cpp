#include "y/lingo/pattern/logic/and.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        And:: And() noexcept :
        Logic(UUID)
        {
        }

        And:: ~And() noexcept {}

        And:: And(const And &other) : Logic(other) {}

        Pattern * And:: clone() const
        {
            return new And(*this);
        }

        bool And:: univocal() const noexcept
        {
            for(const Pattern *p=head;p;p=p->next)
            {
                if(p->feeble() || p->multiple()) return false;
            }
            return true;
        }

        bool And:: strong() const noexcept
        {
            if(size<=0)
                return false;

            for(const Pattern *p=head;p;p=p->next)
            {
                if( p->feeble() )
                    return false;
            }

            return true;
        }

        String And:: regularExpression() const
        {
            String ans = "(";
            for(const Pattern *p=head;p;p=p->next)
            {
                ans += p->regularExpression();
            }
            ans += ")";
            return ans;
        }

        void And:: query(CharDB &fc) const
        {
            for(const Pattern *p=head;p;p=p->next)
            {
                p->query(fc);
                if(p->strong())
                    return;
            }
        }

        bool And:: takes(Token &token, Source &source) const
        {
            assert(0==token.size);

            Token local;
            for(const Pattern *p=head;p;p=p->next)
            {
                Token temp;
                if(p->takes(temp,source))
                {
                    local<<temp;
                }
                else
                {
                    source.put(local);
                    return false;
                }
            }
            local.swapWith(token);
            return true;
        }

    }

}


