#include "y/lingo/syntax/grammar.hpp"
#include "y/system/exception.hpp"

namespace Yttrium {

    namespace Lingo
    {

        namespace Syntax
        {
            XNode * Grammar:: accept(Lexer &lexer, Source &source)
            {
                if(rules.size<=0)
                    throw Specific::Exception(name->c_str(),"emtpy grammar");

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
                const Caption &         label = next->name;
                const Terminal * const  term  = rules.queryTerminal( label );
                if(!term)
                {
                    excp.add("no terminal '%s'", label->c_str());
                    return;
                }
                next->appendTo(excp, term->kind == Terminal::Univocal );
            }


            XNode * Grammar:: accepted(XNode *const node, Lexer &lexer, Source &source)
            {
                assert(0!=node);
                AutoPtr<XNode> guard(node);
                const XNode  &       last = node->last();
                const Lexeme * const next = lexer.peek(source,0);
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
                const Lexeme * const next = lexer.peek(source,0);
                if(0==next)
                    throw Specific::Exception(name->c_str(),"does not accept empty source");
                Specific::Exception excp(name->c_str(),"cannot start with ");
                tryAppendTo(excp, next);
                throw excp;

            }
        }
    }

}
