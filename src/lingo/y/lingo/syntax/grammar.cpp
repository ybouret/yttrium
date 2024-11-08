
#include "y/lingo/syntax/grammar.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            Grammar:: ~Grammar() noexcept
            {
                
            }


            Grammar::ConstInterface & Grammar:: surrogate() const noexcept
            {
                return rules;
            }


            void Grammar:: add(Rule *const rule)
            {
                // check no multiple name
                assert(0!=rule);
                AutoPtr<Rule> pRule = rule; assert( pRule.isValid() );
                const String &rid   = *(rule->name);
                if(locked) throw Specific::Exception(name->c_str(), "locked, trying to add '%s'", rid.c_str());

                for(const Rule *mine=rules.head;mine;mine=mine->next)
                {
                    const String &mid = *(mine->name);
                    if(mid==rid) throw Specific::Exception(name->c_str(),"multiple rule '%s'", rid.c_str());
                }

                // append
                rules.pushTail( pRule.yield() );
            }

            const Rule & Grammar:: top() const
            {
                if(rules.size<=0) throw Specific::Exception(name->c_str(), "no rule in top()");
                return *(rules.head);
            }

            void Grammar:: top(const Rule &rule)
            {
                if(locked)                 throw Specific::Exception(name->c_str(), "locked, trying to top('%s')", rule.name->c_str());
                if( !rules.owns( &rule ) ) throw Specific::Exception(name->c_str(), "unknown rule for top('%s')", rule.name->c_str());
                rules.moveToFront( &Coerce(rule) );
            }



        }

    }

}
