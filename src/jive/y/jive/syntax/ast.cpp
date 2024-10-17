#include "y/jive/syntax/xnode.hpp"
#include "y/jive/syntax/terminal.hpp"
#include "y/jive/syntax/compound/aggregate.hpp"


namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {


            XNode * XNode:: AST(XNode *root) noexcept
            {
                assert(0!=root);
                switch(root->type)
                {
                    case IsTerminal: return astTerminal(root);
                    case IsInternal: return astInternal(root);
                }
                return root;
            }

            XNode *XNode:: astTerminal(XNode *root) noexcept
            {
                assert(0!=root);
                assert(IsTerminal==root->type);

                std::cerr << "astTerminal('" << root->name() << "')" << std::endl;

                const Rule &rule = root->rule;
                if(rule.is<Terminal>())
                {
                    const Terminal &term = *rule.as<Terminal>();
                    switch( term.property )
                    {
                        case Terminal::IsRegular:
                            if(term.univocal) root->lptr->release();
                            return root;

                        case Terminal::IsDivider:
                            if(root->sire)
                            {
                                std::cerr << "divider '" << root->name() << "': sire='" << root->sire->name() << "'" << std::endl;
                                assert(IsInternal==root->sire->type);
                                delete root;
                                return 0;
                            }
                            return root;
                    }
                }

                // otherwise, return untouched
                return root;
            }

            XNode *XNode:: astInternal(XNode *root) noexcept
            {
                assert(0!=root);
                assert(IsInternal==root->type);

                std::cerr << "astInternal('" << root->name() << "')" << std::endl;

                //--------------------------------------------------------------
                //
                // pre-process child(ren)
                //
                //--------------------------------------------------------------
                {
                    XList  target;
                    XList &source = root->chld;
                    while(source.size>0)
                    {
                        AutoPtr<XNode> node = AST( source.popHead() ); if(node.isEmpty()) continue;
                        const Rule &   rule = node->rule;
                        if(rule.is<Aggregate>() )
                        {
                            assert(IsInternal==node->type);
                            switch( rule.as<Aggregate>()->property )
                            {
                                case Aggregate::Permanent:
                                    // do nothing
                                    break;

                                case Aggregate::Transient:
                                    target.mergeTail(node->chld);
                                    continue; // will delete node

                                case Aggregate::Surrogate:
                                    if(node->chld.size<=1)
                                    {
                                        target.mergeTail(node->chld);
                                        continue;
                                    }
                                    break;
                            }
                        }
                        target.pushTail(node.yield());
                    }
                    target.swapWith(source);
                }

                //--------------------------------------------------------------
                //
                // post-process transformed root
                //
                //--------------------------------------------------------------
                {
                    const Rule &rule = root->rule;
                    if(rule.is<Aggregate>()                                   &&
                       rule.as<Aggregate>()->property != Aggregate::Permanent &&
                       1 == root->chld.size)
                    {
                        const AutoPtr<XNode> guard(root);
                        return root->chld.popHead();
                    }
                }

                return root;
            }

        }

    }

}
