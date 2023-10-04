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
            XNode:: XNode() noexcept :
            type(IsInternal),
            next(0),
            prev(0),
            sire(0),
            wksp(),
            list_( *Memory::OutOfReach::Cast<List>(wksp) )
            {
                new ( memset(wksp,0,sizeof(wksp)) ) List();
            }


            XNode:: XNode(Lexeme *lx) noexcept :
            type(IsTerminal),
            next(0),
            prev(0),
            sire(0),
            wksp(),
            list_( *Memory::OutOfReach::Cast<List>(wksp) )
            {
                memset(wksp,0,sizeof(wksp));
                lptr = lx;
            }

            XNode * XNode:: Create()
            {
                return new XNode();
            }

            XNode * XNode:: Create(Lexeme *lx)
            {
                assert(0!=lx);
                AutoPtr<Lexeme> guard(lx);
                XNode *xn = new XNode(lx);
                guard.relax();
                return xn;
            }

            XNode:: ~XNode() noexcept
            {
                switch(type)
                {
                    case IsInternal:
                        Destruct( &list_ );
                        memset(wksp,0,sizeof(wksp));
                        break;

                    case IsTerminal:
                        if(lptr) Nullify(lptr);
                        break;
                }

            }
        }

    }

}

