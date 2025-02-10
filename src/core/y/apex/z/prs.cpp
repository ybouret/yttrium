
#include "y/apex/integer.hpp"
#include "y/system/exception.hpp"
#include "y/container/algo/crop.hpp"

namespace Yttrium
{

    namespace Apex
    {

        Integer Integer:: Parse(const String &str)
        {
            String data = str;
            Algo::Crop(data,isspace);
            switch(data.size())
            {
                case 0: throw Specific::Exception(CallSign,"Parse Blank String");
                case 1: { const Natural n(data); return Integer(n); }
                default:
                    break;
            }
            SignType theSign = Positive;
            if('-'==data.head())
            {
                theSign = Negative;
                data.skip(1);
            }
            const Natural n(data);
            if(n->bits<=0) theSign = __Zero__;
            return Integer(theSign,n);
        }

        Integer & Integer:: operator=(const String &str)
        {
            Integer _ = Parse(str);
            return xch(_);
        }

        Integer:: Integer(const String &str) :
        Castable(), s(__Zero__), n(0)
        {
            Integer _ = Parse(str);
            (void) xch(_);
        }
    }

}
