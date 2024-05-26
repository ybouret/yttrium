#include "y/apk/element.hpp"



namespace Yttrium
{
    namespace APK
    {
        InnerState Element:: TuneUp(Element &lhs, Element &rhs) noexcept
        {
            switch( Sign::Of(lhs.state,rhs.state) )
            {
                case Negative: lhs.set(rhs.state); break;
                case __Zero__: break;
                case Positive: rhs.set(lhs.state); break;
            }
            assert(lhs.state==rhs.state);
            return lhs.state;
        }

    }

}

