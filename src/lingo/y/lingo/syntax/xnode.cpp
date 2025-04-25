
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

            bool XNode:: is(const char * const id) const noexcept
            {
                return id == name();
            }

            bool XNode:: is(const String &id) const noexcept
            {
                return id == name();
            }

           

        }
    }
}

#include "y/lingo/syntax/rule.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            bool XNode:: AreEqual(const XNode &lhs,
                                  const XNode &rhs)
            {
                if( &lhs.rule != &rhs.rule ) return false;

                switch( lhs.type )
                {
                    case Terminal: return lhs.lexeme() == rhs.lexeme();
                    case Internal:
                        break;
                }

                for(const XNode
                    *l=lhs.branch().head,
                    *r=rhs.branch().head;
                    l;
                    l=l->next,r=r->next)
                {
                    assert(0!=l);
                    assert(0!=r);
                    if( ! AreEqual(*l,*r) ) return false;
                }

                return true;

            }


            bool operator==(const XNode &lhs, const XNode &rhs)
            {
                return XNode:: AreEqual(lhs,rhs);
            }

            bool operator!=(const XNode &lhs, const XNode &rhs)
            {
                return !XNode:: AreEqual(lhs,rhs);
            }

        }

    }

}


