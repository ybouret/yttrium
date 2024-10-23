
#include "y/lingo/pattern/regexp/compiler.hpp"
#include "y/lingo/pattern/all.hpp"
#include "y/system/exception.hpp"

#include <cstring>

namespace Yttrium
{
    namespace Lingo
    {
        static const char escRaw[] = "(){}[]+*?&\\/'\"";




        Pattern * RXC:: subExprEsc()
        {
            const char * const start = curr-1;
            assert('\\'==*start);


            if(curr>=last) throw Specific::Exception(CallSign, "unfinished escaped sequence in sub-expression");
            const char c = *(curr++);

            if( 0 != strchr(escRaw,c) ) return new Byte(c);

            throw Specific::Exception(CallSign, "unknow escape sequence '\\%c...'",c);
        }
    }

}


