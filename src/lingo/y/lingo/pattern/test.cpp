#include "y/lingo/pattern/all.hpp"
#include "y/system/exception.hpp"


namespace Yttrium
{
    namespace Lingo
    {

        
        bool operator==(const Pattern &lhs, const Pattern &rhs)
        {
            return Pattern::Equality(lhs,rhs);
        }

        bool Pattern::Equality(const Pattern &lhs, const Pattern &rhs)
        {
            const uint32_t uuid = lhs.uuid; if(uuid!=rhs.uuid) return false;

            assert(lhs.uuid==rhs.uuid);
            switch(uuid)
            {
                    // basic
                case Any1::UUID:
                case Void::UUID:
                    return true;

                case Byte::    UUID: return lhs.as<Byte>()->byte  == rhs.as<Byte>()->byte;
                case Exclude:: UUID: return lhs.as<Exclude>()->byte == rhs.as<Exclude>()->byte;
            }

            const String l = lhs.toBinary();
            const String r = rhs.toBinary();
            return l == r;
        }

    }

}
