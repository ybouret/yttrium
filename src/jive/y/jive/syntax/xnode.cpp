#include "y/jive/syntax/xnode.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/type/nullify.hpp"
#include "y/ptr/auto.hpp"

#include <cstring>

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {
            XNode:: XNode(const Rule &from) noexcept :
            rule(from),
            type(IsInternal),
            next(0),
            prev(0),
            sire(0),
            wksp(),
            chld( *Memory::OutOfReach::Cast<List>(wksp) )
            {
                new ( memset(wksp,0,sizeof(wksp)) ) List();
            }


            XNode:: XNode(const Rule &from, Lexeme *lx) noexcept :
            rule(from),
            type(IsTerminal),
            next(0),
            prev(0),
            sire(0),
            wksp(),
            chld( *Memory::OutOfReach::Cast<List>(wksp) )
            {
                memset(wksp,0,sizeof(wksp));
                lptr = lx;
            }


            size_t XNode:: weight() const noexcept
            {
                switch(type)
                {
                    case IsTerminal: return 1;
                    case IsInternal: break;
                }
                size_t w = 0;
                for(const XNode *sub=chld.head;sub;sub=sub->next)
                {
                    assert(this==sub->sire);
                    w += sub->weight();
                }
                return w;
            }

            const Lexeme & XNode:: lexeme() const noexcept
            {
                assert(IsTerminal==type);
                assert(0!=lptr);
                return *lptr;
            }

            const XList & XNode:: branch() const noexcept
            {
                assert(IsInternal==type);
                return chld;
            }

            void XNode:: fusion(Tree &tree) noexcept
            {
                assert(tree.isValid());
                assert(IsInternal==type);
                switch(tree->type)
                {
                    case IsTerminal:
                        chld.pushTail( tree.yield() );
                        break;

                    case IsInternal:
                        chld.mergeTail( tree->chld );
                        break;
                }
            }


            XNode:: ~XNode() noexcept
            {
                switch(type)
                {
                    case IsInternal:
                        Destruct( &chld );
                        memset(wksp,0,sizeof(wksp));
                        break;

                    case IsTerminal:
                        if(lptr) Nullify(lptr);
                        break;
                }

            }

            void XNode:: Expand(Tree &tree, XNode *node) noexcept
            {
                assert(0!=node);
                assert(0==node->sire);

                if(tree.isValid())
                {
                    assert(tree->type == IsInternal);
                    tree->chld.pushTail(node);
                    node->sire = & *tree;
                }
                else
                {
                    tree = node;
                }
            }

            void XNode:: Expand(Tree &tree, Tree &sub) noexcept
            {
                if(sub.isValid())
                    Expand(tree,sub.yield());
            }


            void XNode:: BackTo(Lexer &lexer, XNode *node) noexcept
            {
                assert(0!=node);
                
                switch(node->type)
                {
                    case IsInternal:
                        while(node->chld.size)
                            BackTo(lexer,node->chld.popTail());
                        break;

                    case IsTerminal:
                        assert(0!=node->lptr);
                        lexer.put(node->lptr);
                        node->lptr = 0;
                        break;
                }

                delete node;
            }

            void XNode::BackTo(Lexer &lexer, Tree &tree) noexcept
            {
                BackTo(lexer,tree.yield());
            }

        }

    }

}


#include "y/jive/syntax/terminal.hpp"
#include "y/jive/syntax/internal.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            XNode * XNode:: Create(const Internal &from)
            {
                return new XNode(from);
            }

            XNode * XNode:: Create(const Terminal &from, Lexeme *lx)
            {
                assert(0!=lx);
                AutoPtr<Lexeme> guard(lx);
                XNode *xn = new XNode(from,lx);
                guard.relax();
                return xn;
            }

            void XNode:: remove(const Tag &tag) noexcept
            {
                assert(IsInternal==type);
                XList  target;
                XList &source = chld;
                while(source.size>0)
                {
                    AutoPtr<XNode> node = source.popHead();
                    if(*(node->rule.name) == *tag)
                        continue;
                    target.pushTail(node.yield());
                }
                source.swapWith(target);
            }
            
        }
    }
}

