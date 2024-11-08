
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
                assert(0!=rule);
                AutoPtr<Rule> pRule = rule;
                const String &rid   = *(rule->name);
                if(locked) throw Specific::Exception(name->c_str(), "grammar is locked, trying to add '%s'", rid.c_str());

                for(const Rule *mine=rules.head;mine;mine=mine->next)
                {
                    const String &mid = *(mine->name);
                    if(mid==rid) throw Specific::Exception(name->c_str(),"multiple rule '%s'", rid.c_str());
                }

                rules.pushTail( pRule.yield() );
            }

        }

    }

}
