
#include "y/lingo/syntax/xnode.hpp"

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


            XNode * XNode:: CreateFrom(const Rule &rule, Lexeme * const unit)
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


            XNode * XNode:: CreateFrom(const Rule &rule)
            {
                return new XNode(rule);
            }

            

        }

    }

}


