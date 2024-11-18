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

        bool And:: _univocal() const
        {
            for(const Pattern *p=head;p;p=p->next)
            {
                if(p->feeble() || p->multiple()) return false;
            }
            return true;
        }

        bool And:: _strong() const
        {
            for(const Pattern *p=head;p;p=p->next)
            {
                if( p->strong() )
                    return true;
            }
            return false;

        }

        String And:: _regexp() const
        {
            if(1==size)
                return head->regularExpression();
            else
            {
                String ans = "(";
                for(const Pattern *p=head;p;p=p->next)
                {
                    ans += p->regularExpression();
                }
                ans += ")";
                return ans;
            }
        }

        void And:: _query(CharDB &fc) const
        {
            assert(size>0);
            for(const Pattern *p=head;p;p=p->next)
            {
                p->query(fc);
                if(p->strong())
                    return;
            }

        }

        bool And:: _takes(Y_Lingo_Pattern_Args) const
        {
            assert(0==token.size);
            assert(size>0);
            
            Token local;
            for(const Pattern *p=head;p;p=p->next)
            {
                Token temp;
                if(p->takes(temp,source,0))
                {
                    local<<temp;
                }
                else
                {
                    source.put(local);
                    return false;
                }
            }

            // all accepted
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


