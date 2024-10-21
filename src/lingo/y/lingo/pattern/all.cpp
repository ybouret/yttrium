#include "y/lingo/pattern/all.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        bool Pattern:: isBasic() const noexcept
        {
            switch(uuid)
            {
                case Any1::    UUID:
                case Void::    UUID:
                case Single::  UUID:
                case Range::   UUID:
                case Exclude:: UUID:
                    return true;
                default:
                    break;
            }
            return false;
        }


        bool Pattern:: isJoker() const noexcept
        {
            switch(uuid)
            {
                case Optional:: UUID:
                case Repeated:: UUID:
                case Counting:: UUID:
                    return true;
                default:
                    break;
            }
            return false;
        }


        bool Pattern:: isLogic() const noexcept
        {
            switch(uuid)
            {
                case And::  UUID:
                case Or::   UUID:
                case None:: UUID:
                    return true;
                default:
                    break;
            }
            return false;
        }


    }

}
