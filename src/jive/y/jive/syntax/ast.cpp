#include "y/jive/syntax/xnode.hpp"
#include "y/jive/syntax/terminal.hpp"


namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {


            XNode * XNode:: astTerminal(XNode *root) noexcept
            {
                assert(0!=root);
                assert(IsTerminal == root->type);

                const Rule &r = root->rule;
                if(r.is<Terminal>())
                {
                    const Terminal &t = * r.as<Terminal>();
                    switch(t.property)
                    {
                        case Terminal::IsDivider:
                            delete root;
                            return 0;

                        case Terminal::IsRegular:
                            if(t.univocal) root->lptr->release();
                            return root;
                    }
                }

                return root;
            }

            XNode * XNode:: astInternal(XNode *root) noexcept
            {
                assert(0!=root);
                assert(IsInternal == root->type);

                XList   target;
                XList & source = root->chld;
                while(source.size>0)
                {
                    XNode *node = source.popHead();
                    switch(node->type)
                    {
                        case IsTerminal:
                            if( 0 != (node=astTerminal(node)) ) target.pushTail(node);
                            break;

                        case IsInternal:
                            target.pushTail( astInternal(node) );
                            break;
                    }
                }
                source.swapWith(target);
                return root;
            }

            XNode * XNode:: AST(XNode *root) noexcept
            {
                assert(0!=root);

                switch(root->type)
                {
                    case IsTerminal: return astTerminal(root);
                    case IsInternal: break;
                }
                return astInternal(root);
            }

        }

    }

}
