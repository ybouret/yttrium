#include "y/lingo/syntax/rules.hpp"

namespace Yttrium {

    namespace Lingo
    {

        namespace Syntax
        {

            namespace
            {



                static inline
                XNode * InternalAST(XNode * const tree) noexcept
                {
                    assert(0!=tree);
                    XList &list = tree->branch();
                    XList temp;
                    while(list.size>0)
                    {
                        //------------------------------------------------------
                        //
                        //
                        // recursive transformation
                        //
                        //
                        //------------------------------------------------------
                        AutoPtr<XNode> node = XNode::AST( list.popHead() );

                        //------------------------------------------------------
                        //
                        //
                        // post process fetched node
                        //
                        //------------------------------------------------------

                        switch(node->type)
                        {
                                //----------------------------------------------
                                //
                                // post-process Terminal
                                //
                                //----------------------------------------------
                            case XNode:: Terminal:
                                switch(node->rule.as<Terminal>()->role)
                                {
                                    case Syntax::Terminal::Semantic:
                                        break;
                                    case Syntax::Terminal::Dividing:
                                        continue; // will erase node
                                } break;

                                //----------------------------------------------
                                //
                                // post-process internal
                                //
                                //----------------------------------------------
                            case XNode::Internal:
                                switch(node->rule.uuid)
                                {
                                    case Aggregate::UUID:
                                        switch(node->rule.as<Aggregate>()->type)
                                        {
                                            case Aggregate::Definite:
                                                break;

                                            case Aggregate::Grouping:
                                                tree->fusion(node->branch());
                                                assert(tree->isWellFormed());
                                                continue; // will erase node

                                            case Aggregate::NoSingle:
                                                if(1==node->branch().size)
                                                {
                                                    tree->fusion(node->branch());
                                                    assert(tree->isWellFormed());
                                                    continue;
                                                }
                                                else
                                                    break; // stay this way

                                        }
                                        break;

                                    default: // other internal...
                                        break;
                                }
                                break;

                        }
                        temp.pushTail(node.yield());
                    }
                    list.swapWith(temp);

                    assert(tree->isWellFormed());
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


