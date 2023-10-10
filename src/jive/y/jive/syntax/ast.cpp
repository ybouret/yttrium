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

                XList  target;
                XList &source = root->chld;
                while(source.size>0)
                {
                    AutoPtr<XNode> node = AST( source.popHead() );

                    if(node.isEmpty()) continue;
                    const Rule &rule = node->rule;
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

                return root;
            }

        }

    }

}
