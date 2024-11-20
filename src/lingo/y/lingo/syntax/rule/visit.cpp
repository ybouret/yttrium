
#include "y/lingo/syntax/rule/visit.hpp"
#include "y/lingo/syntax/rules.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {


            
            Visit:: Visit() : AddressBook()
            {
            }

            Visit:: Visit(const Rule &rule) : AddressBook()
            {
                (*this)(rule);
            }

            Visit:: ~Visit() noexcept
            {
            }

            static const char Function[] = "Syntax::Visit";


            static inline
            void walk( Visit &visit, const Manifest &manifest)
            {
                for(const RNode *node=manifest.head;node;node=node->next)
                    visit(**node);
            }

            void Visit:: operator()(const Rule &rule)
            {
                Visit &visit = *this;

                if( insert(rule) )
                {
                    // rule was not visited
                    const uint32_t uuid = rule.uuid;

                    switch(uuid)
                    {
                        case Terminal::  UUID: break;
                        case Option::    UUID: visit(rule.as<Option>()->rule); break;
                        case Repeat::    UUID: visit(rule.as<Repeat>()->rule); break;
                        case Aggregate:: UUID: walk(visit,**rule.as<Aggregate>()); break;
                        case Alternate:: UUID: walk(visit,**rule.as<Alternate>()); break;

                        default:
                            throw Specific::Exception(Function, "unhandled UUID='%s' for '%s'", FourCC::ToText(uuid), rule.name->c_str());
                    }
                }
            }

            bool Visit:: Found(const Rule &root, const Rule &rule)
            {
                const Visit visit(rule);
                return visit.has(root);
            }



        }

    }

}

