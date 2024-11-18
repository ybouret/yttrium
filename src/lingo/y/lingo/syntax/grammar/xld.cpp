
#include "y/lingo/syntax/grammar.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            static inline
            XNode * reloadTerminal(const Terminal &   rule,
                                   Source         &   source,
                                   const Grammar  &   grammar,
                                   const Char     &   here)
            {
                const char * const name  = rule.name->c_str();
                const size_t       size  = source.readVBR<size_t>(name);

                Token token;
                for(size_t i=1;i<=size;++i)
                {
                    Char * const ch = source.get();
                    if(!ch) throw Specific::Exception(grammar.name->c_str(),"EOF reading #%u/%u of '%s'", unsigned(i), unsigned(size), name);
                    token << ch;
                }

                AutoPtr<Lexeme> lx = new Lexeme(rule, (0!=token.head) ? *(token.head) : here);
                lx->swapWith(token);

                return XNode::CreateFrom(rule,lx.yield());
            }


            static inline
            XNode * reloadInternal(const Internal &rule,
                                   Source         &source,
                                   const Grammar  &grammar)
            {
                AutoPtr<XNode>     node  = XNode::CreateFrom(rule);
                const char * const name  = rule.name->c_str();
                const size_t       size  = source.readVBR<size_t>(name);
                XList             &target = node->branch();

                for(size_t i=1;i<=size;++i)
                    target.pushTail( grammar.reload(source) );
                
                return node.yield();
            }


            XNode * Grammar:: reload(Source &source) const
            {
                const Grammar &self = *this;  if(!source.ready()) throw Specific::Exception(self.name->c_str(),"EOF in reload()");
                const Char     here = *source.peek();
                const String   name = String::ReadFrom(source);
                const Rule    &rule = self[name];

                switch(rule.uuid)
                {
                    case Terminal::UUID:  return reloadTerminal( *rule.as<Terminal>(),  source, *this, here);
                    case Aggregate::UUID: return reloadInternal( *rule.as<Aggregate>(), source, *this);
                    default:
                        break;
                }

                throw Specific::Exception(name.c_str(), "reload of rule is not implemented");
            }
        }

    }

}

