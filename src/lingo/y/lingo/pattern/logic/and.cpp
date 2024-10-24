#include "y/lingo/pattern/logic/and.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Y_Lingo_Pattern_CallSign(And)


        And:: And() noexcept : Logic(UUID)
        {
            Y_Lingo_Pattern(And);
        }

        And:: ~And() noexcept {}

        And:: And(const And &other) : Logic(other)
        {
            Y_Lingo_Pattern(And);
        }

        Pattern * And:: clone() const
        {
            return new And(*this);
        }

        bool And:: univocal() const noexcept
        {
            if(size<=0)
            {
                return true;
            }
            else
            {
                for(const Pattern *p=head;p;p=p->next)
                {
                    if(p->feeble() || p->multiple()) return false;
                }
                return true;
            }
        }

        bool And:: strong() const noexcept
        {
            if(size<=0)
            {
                // degenerate
                return true;
            }
            else
            {
                for(const Pattern *p=head;p;p=p->next)
                {
                    if( !p->strong() )
                        return false;
                }
                return true;
            }
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

        Pattern * Pattern:: Exact(const String &str)
        {
            const size_t n = str.size();
            if(n<=0) throw Specific::Exception("Pattern::Exact","empty string");

            AutoPtr<Logic> p = new And();
            for(size_t i=1;i<=n;++i)
            {
                p->add( str[i] );
            }
            return p.yield();
        }


        void And:: viz(OutputStream &fp) const
        {
            for(const Pattern *node=head;node;node=node->next)
            {
                node->viz(fp);
            }

            Node(fp,this) << '[';
            Label(fp, "&") << ",shape=house";
            Endl(fp << ']');
            
            vizLink(fp);
        }

    }

}


