
#include "y/lingo/lexical/unit.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {
            Unit:: ~Unit() noexcept {}

            Unit:: Unit(const Entity  &_root,
                        const Context &_cntx) noexcept :
            Object(),
            Entity(_root),
            Token(),
            next(0),
            prev(0),
            info(_cntx)
            {

            }

            Unit:: Unit(const Unit &_) :
            Object(),
            Entity(_),
            Token(_),
            next(0),
            prev(0),
            info(_.info)
            {
            }
            

            std::ostream & operator<<(std::ostream &os, const Unit &unit)
            {
                os << unit.name;
                if(unit.size>0) os << '=' << '\'' << unit.toPrintable() << '\'';
                return os;
            }

            void Unit:: appendTo(Exception &excp, const bool univocal) const noexcept
            {
                excp.add("'%s'", name->c_str());
                if(!univocal)
                {
                    const String s = toPrintable();
                    excp.add("='%s'",s.c_str());
                }
            }

        }
        
    }

}
