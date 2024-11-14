#include "y/lingo/syntax/grammar.hpp"

namespace Yttrium {

    namespace Lingo
    {

        namespace Syntax
        {

            namespace
            {
                static inline
                XList & applyAST( XList &list ) noexcept
                {
                    XList temp;
                    while(list.size>0)
                    {
                        // recursive transformation
                        AutoPtr<XNode> node = XNode::AST( list.popHead() );

                        // post process fetched node
                        switch(node->type)
                        {
                            case XNode:: Terminal:
                                switch(node->rule.as<Terminal>()->role)
                                {
                                    case Syntax::Terminal::Semantic:
                                        break;
                                    case Syntax::Terminal::Dividing:
                                        continue;
                                } break;

                            case XNode::Internal:

                                break;

                        }
                        temp.pushTail(node.yield());
                    }
                    list.swapWith(temp);
                    return list;
                }

                static inline
                XNode * InternalAST(XNode * const tree) noexcept
                {
                    assert(0!=tree);
                    XList &list = applyAST( tree->branch() );
                    
                    return tree;
                }


                static inline
                XNode * TerminalAST(XNode * const node) noexcept
                {
                    assert(XNode::Terminal==node->type);
                    const Terminal &term = *(node->rule.as<Terminal>());
                    switch(term.kind)
                    {
                        case Terminal::Univocal: node->lexeme().release(); break;
                        case Terminal::Standard: break;
                    }
                    return node;
                }

            }

            XNode * XNode:: AST( XNode * const tree ) noexcept
            {
                assert(0!=tree);
                switch(tree->type)
                {
                    case XNode::Internal: return InternalAST(tree);
                    case XNode::Terminal: return TerminalAST(tree);
                }
            }

        }

    }

}


