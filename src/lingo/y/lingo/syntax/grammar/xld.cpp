
#include "y/lingo/syntax/grammar.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            static inline
            XNode * reloadTerminal(const Terminal &rule,
                                   Source         &source,
                                   const Grammar  &grammar)
            {
                const char * const id    = rule.name->c_str();
                const size_t       count = source.readVBR<size_t>(id);
                if(count<=0)
                    throw Specific::Exception(grammar.name->c_str(), "empty '%s'",id);

                Token token;
                for(size_t i=1;i<=count;++i)
                {
                    Char * const ch = source.get();
                    if(!ch) throw Specific::Exception(grammar.name->c_str(),"EOF reading '%s'", id);
                    token << ch;
                }

                AutoPtr<Lexeme> lx = new Lexeme(rule,*(token.head));
                lx->swapWith(token);

                return XNode::CreateFrom(rule,lx.yield());
            }


            static inline
            XNode * reloadInternal(const Internal &rule,
                                   Source         &source,
                                   const Grammar  &grammar)
            {
                AutoPtr<XNode>     xnode  = XNode::CreateFrom(rule);
                const char * const id     = rule.name->c_str();
                const size_t       count  = source.readVBR<size_t>(id);
                XList             &target = xnode->branch();
                for(size_t i=1;i<=count;++i)
                {
                    target.pushTail( grammar.reload(source) );
                }
                return xnode.yield();
            }


            XNode * Grammar:: reload(Source &source) const
            {
                const Grammar &self     = *this;
                const String   ruleName = String::ReadFrom(source);
                const Rule    &rule     = self[ruleName];

                switch(rule.uuid)
                {
                    case Terminal::UUID:  return reloadTerminal( *rule.as<Terminal>(),  source, *this);
                    case Aggregate::UUID: return reloadInternal( *rule.as<Aggregate>(), source, *this);
                    default:
                        break;
                }

                throw Specific::Exception(name->c_str(), "cannot reload rule '%s'", ruleName.c_str());
            }
        }

    }

}

