
#include "y/kemp/integer.hpp"
#include "y/stream/output.hpp"
#include "y/stream/input.hpp"
#include "y/system/exception.hpp"

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

        Integer Integer:: ReadFrom(InputStream &fp)
        {
            const uint8_t mark = fp.readCBR<uint8_t>("sign");
            SignType      s    = __Zero__;
            switch(mark)
            {
                case NegativeMark: s = Negative; break;
                case PositiveMark: s = Positive; break;
                case __Zero__Mark: return Natural();
                default:
                    throw Specific::Exception(CallSign, "unknown sign mark 0x%02x",mark);
            }
            const Natural n = Natural::ReadFrom(fp);
            return Integer(s,n);
        }

    }

}

