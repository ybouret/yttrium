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

            void Grammar:: tryAppendTo(Exception &          excp,
                                       const char   * const prefix,
                                       const Lexeme * const lexeme) const
            {
                assert(0!=prefix);
                const Caption &         label = lexeme->name;
                const Terminal * const  term  = rules.queryTerminal( label );
                if(!term)
                {
                    excp.add("unknown terminal '%s'", label->c_str());
                    return;
                }
                excp.add("%s ",prefix);
                lexeme->appendTo(excp, term->kind == Terminal::Univocal );
            }


            XNode * Grammar:: accepted(XNode *const node, Lexer &lexer, Source &source)
            {
                assert(0!=node);
                AutoPtr<XNode> guard(node);
                const Lexeme * const last = node->lastLexeme();
                const Lexeme * const next = lexer.peek(source,last);
                if(0!=next)
                {
                    // unexpected/extraneous
                    Specific::Exception excp(name->c_str(),"extraneous ");
                    tryAppendTo(excp, "terminal", next);
                    if(last)
                    {
                        tryAppendTo(excp, " after",last);
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
                tryAppendTo(excp, "terminal", next);
                throw excp;

            }
        }
    }

}
