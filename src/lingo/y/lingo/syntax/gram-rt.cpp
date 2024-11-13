#include "y/lingo/syntax/grammar.hpp"
#include "y/system/exception.hpp"

namespace Yttrium {

    namespace Lingo
    {

        namespace Syntax
        {
            XNode * Grammar:: accept(Lexer &lexer, Source &source)
            {
                if(rules.size<=0) throw Specific::Exception(name->c_str(),"emtpy grammar");
                const Rule &primary = *(rules.head);
                XNode *     tree    = 0;
                if(primary.accepts(lexer,source,tree) )
                {
                    
                    return accepted(tree,lexer,source);
                }
                else
                {
                    assert(0==tree);
                    rejected(lexer,source);
                    return tree;
                }

            }

            XNode * Grammar:: accepted(XNode *const node, Lexer &lexer, Source &source)
            {
                assert(0!=node);
                AutoPtr<XNode> guard(node);
                const XNode  &       last = node->last();
                const Lexeme * const next = lexer.peek(source);
                if(0!=next)
                {
                    // unexpected/extraneous
                    const Caption &     label = next->name;
                    const Rule * const  rule  = query( *label );
                    Specific::Exception excp(name->c_str(),"extraneous ");
                    if(!rule)
                    {
                        excp.add("not registered '%s'", label->c_str());
                        goto AFTER;
                    }
                    if(!rule->isTerminal())
                    {
                        excp.add("terminal '%s' with internal '%s'", label->c_str(), rule->name->c_str());
                        goto AFTER;
                    }
                    next->appendTo(excp,rule->as<Terminal>()->kind == Terminal::Univocal );
                AFTER:
                    if(last.type==XNode::Terminal)
                    {
                        excp.add(" after ");
                        last.appendTo(excp);
                    }
                    throw excp;
                }
                return guard.yield();
            }

            void Grammar:: rejected(Lexer &lexer, Source &source)
            {
                
            }
        }
    }

}
