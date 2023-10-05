#include "y/jive/syntax/grammar.hpp"
#include "y/type/nullify.hpp"
#include "y/system/exception.hpp"
#include "y/associative/suffix/set.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {


            class Grammar:: Code : public Object, public Rules
            {
            public:
                typedef ArkPtr<String,Rule> RulePtr;

                explicit Code(const Tag &id) noexcept:
                Object(),
                Rules(),
                name(*id),
                entry(0)
                {
                }

                virtual ~Code() noexcept
                {
                    
                }

                void add(Rule *rule)
                {
                    assert(0!=rule);
                    {
                        AutoPtr<Rule> guard(rule);
                        const String &ruleName = *(rule->name);
                        for(const Rule *mine=head;mine;mine=mine->next)
                        {
                            const String &rid = *(mine->name);
                            if(rid==ruleName)
                                throw Specific::Exception(name.c_str(), "multiple rule [%s]",ruleName.c_str());
                        }
                        pushTail(guard.yield());
                    }

                    //const RulePtr tmp(rule);

                }

                const String &name;
                Rule         *entry;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };

            Grammar::Code * Grammar::Initialize(const Tag &id)
            {
                return new Code(id);
            }
        }

    }

}


namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            Grammar:: ~Grammar() noexcept
            {
                assert(0!=code);
                Nullify(code);
            }

            void Grammar:: add(Rule *rule)
            {
                assert(0!=rule);
                assert(0!=code);
                code->add(rule);
            }
        }

    }

}

