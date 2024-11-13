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

            void Grammar:: tryAppendTo(Exception &excp, const Lexeme * const next) const
            {
                const Caption &     label = next->name;
                const Rule * const  rule  = query( *label );
                if(!rule)
                {
                    excp.add("not registered '%s'", label->c_str());
                    return;
                }

                if(!rule->isTerminal())
                {
                    excp.add("terminal '%s' with internal '%s'", label->c_str(), rule->name->c_str());
                }

                next->appendTo(excp,rule->as<Terminal>()->kind == Terminal::Univocal );


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
                    Specific::Exception excp(name->c_str(),"extraneous ");
                    tryAppendTo(excp, next);
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
                const Lexeme * const next = lexer.peek(source);
                if(0==next)
                {
                    throw Specific::Exception(name->c_str(),"does not accept empty source");
                }

                Specific::Exception excp(name->c_str(),"cannot start with ");
                tryAppendTo(excp, next);
                throw excp;

            }
        }
    }

}
