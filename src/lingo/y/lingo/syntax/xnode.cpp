
#include "y/lingo/syntax/xnode.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

          
            void    XNode:: zero() noexcept
            {
                memset(wksp,0,sizeof(wksp));
                assert(0==unit);
            }


            XList & XNode:: list() const noexcept
            {
                assert(Internal==type);
                const void *  addr = Memory::OutOfReach::Addr(wksp);
                const XList & xref = *static_cast<const XList *>(addr);
                return Coerce(xref);
            }


            XNode * XNode:: CreateFrom(const Syntax::Terminal &rule, Lexeme * const unit)
            {
                assert(0!=unit);
                try {
                    return new XNode(rule,unit);
                }
                catch(...)
                {
                    delete unit;
                    throw;
                }
            }


            XNode * XNode:: CreateFrom(const Syntax::Internal &rule)
            {
                return new XNode(rule);
            }

            
            Lexeme & XNode:: lexeme() noexcept
            {
                assert(Terminal==type);
                assert(0!=unit);
                return *unit;
            }

            const Lexeme & XNode:: lexeme() const noexcept
            {
                assert(Terminal==type);
                assert(0!=unit);
                return *unit;
            }

            XList & XNode:: branch() noexcept
            {
                assert(Internal==type);
                return list();
            }

            const XList & XNode:: branch() const noexcept
            {
                assert(Internal==type);
                return list();
            }

            

        }

    }

}


