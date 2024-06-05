
#include "y/kemp/integer.hpp"
#include "y/stream/output.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        static const uint8_t PositiveMark = 0x0f;
        static const uint8_t __Zero__Mark = 0x00;
        static const uint8_t NegativeMark = 0xf0;

        size_t Integer:: serialize(OutputStream &fp) const
        {
            switch(s)
            {
                case __Zero__: fp.issue(__Zero__Mark); return 1;
                case Negative: fp.issue(NegativeMark); break;
                case Positive: fp.issue(PositiveMark); break;
            }
            return 1+n.serialize(fp);
        }

    }

}

