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

            void XNode:: BackTo(Lexer &lexer, XNode *node) noexcept
            {
                assert(0!=node);
                switch(node->type)
                {
                    case IsTerminal:
                        while(node->chld.size)
                            BackTo(lexer,node->chld.popTail());
                        break;

                    case IsInternal:
                        assert(0!=node->lptr);
                        lexer.put(node->lptr);
                        node->lptr = 0;
                        break;
                }

                delete node;

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
        }
    }
}

