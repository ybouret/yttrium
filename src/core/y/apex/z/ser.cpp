

#include "y/apex/integer.hpp"
#include "y/io/stream/output.hpp"
#include "y/io/stream/input.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{

    namespace Apex
    {

        static const uint8_t Is0 = 0;
        static const uint8_t IsP = 1;
        static const uint8_t IsN = 2;


        size_t Integer:: serialize(OutputStream &fp) const
        {
            size_t           written = 0;
            switch(s)
            {
                case Negative: written += fp.emitCBR(IsN); break;
                case __Zero__: written += fp.emitCBR(Is0); break;
                case Positive: written += fp.emitCBR(IsP); break;
            }
            return written + n.serialize(fp);
        }

        Integer Integer:: ReadFrom(InputStream &fp)
        {
            SignType S = __Zero__;
            switch( fp.readCBR<uint8_t>("apz.sign") )
            {
                case Is0: S = __Zero__; break;
                case IsP: S = Positive; break;
                case IsN: S = Negative; break;
                default:
                    throw Specific::Exception(CallSign,"ReadFrom: invalid sign");
            }
            const Natural N = Natural::ReadFrom(fp);
            return Integer(S,N);
        }
    }

}


