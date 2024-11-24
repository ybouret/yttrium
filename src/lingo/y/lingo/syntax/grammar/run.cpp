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
                if(primary.accepts(lexer,source,tree,0) )
                {
                    if(0==tree) throw Specific::Exception(name->c_str(), "accepted a NULL tree");
                    return accepted(tree,lexer,source);
                }
                else
                {
                    assert(0==tree);
                    rejected(lexer);
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


            XNode * Grammar:: accepted(XNode * const node, Lexer &lexer, Source &source)
            {
                assert(0!=node);
                AutoPtr<XNode>       keep = node;
                GraphViz::Vizible::DotToPng("raw-tree.dot", *keep);

                const Lexeme * const last = node->lastLexeme();
                const Lexeme * const next = lexer.peek(source,last);
                if(0!=next)
                {
                    // unexpected/extraneous
                    Specific::Exception excp(name->c_str(),"unexpected");
                    tryAppendTo(excp, "", next);
                    if(0!=last)
                    {
                        tryAppendTo(excp, " after",last);
                    }
                    next->info.stamp(excp);
                    throw excp;
                }
                return keep.yield();
            }

            void Grammar:: rejected(const Lexer &lexer)
            {
                const Lexeme * const curr = lexer.tail();
                if(0==curr)
                    throw Specific::Exception(name->c_str(),"does not accept empty source");

                const Lexeme * const prev = curr->prev;
                if(0==curr->prev)
                {
                    Specific::Exception excp(name->c_str(),"cannot start with");
                    tryAppendTo(excp, " ", curr);
                    curr->info.stamp(excp);
                    throw excp;
                }
                else
                {
                    Specific::Exception excp(name->c_str(),"invalid");
                    tryAppendTo(excp, " ", curr);
                    tryAppendTo(excp, " after", prev);
                    curr->info.stamp(excp);
                    throw excp;
                }

            }
        }
    }

}
