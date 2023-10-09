
#include "y/jive/lexical/unit.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Lexical
        {
            Unit:: ~Unit() noexcept {}

            Unit:: Unit(const Entity  &ent,
                        const Context &ctx) noexcept :
            Entity(ent),
            Context(ctx),
            Token(),
            next(0),
            prev(0)
            {
            }
            

            Unit:: Unit(const Unit &other)  :
            Entity( static_cast<const Entity&>(other) ),
            Context(other),
            Token(other),
            next(0),
            prev(0)
            {
            }

            std::ostream & operator<<(std::ostream &os, const Unit &unit)
            {
                os << "@"   << unit.tag  << ':' << unit.line << ':' << unit.column;
                os << ": '" << unit.name << "'='" << unit.toPrintable() << "'";
                return os;
            }

            void Unit:: publishIn(Exception &excp) const
            {
                if(size>0)
                {
                    const String data = toPrintable();
                    excp.add("'%s'='%s'", name->c_str(), data.c_str());
                }
                else
                {
                    excp.add("'%s'",name->c_str());
                }
            }

        }
        


    }

}

