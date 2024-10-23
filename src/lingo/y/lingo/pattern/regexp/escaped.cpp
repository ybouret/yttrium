
#include "y/lingo/pattern/regexp/compiler.hpp"
#include "y/lingo/pattern/all.hpp"
#include "y/system/exception.hpp"

#include <cstring>

namespace Yttrium
{
    namespace Lingo
    {
        static const char escExprChar[] = "(){}[]+*?&\\/'\"";
        static const char escCntlChar[] = "0abfnrtv";
        static const char escCntlCode[] = "\0\a\b\f\n\r\t\v";


        static inline bool isExprChar(const char c, AutoPtr<Pattern> &m )
        {
            assert( m.isEmpty() );

            if(0!=strchr(escExprChar,c))
            {
                m = new Byte(c);
                return true;
            }
            else
            {
                return false;
            }
        }

        static inline bool isCntlChar(const char c, AutoPtr<Pattern> &m)
        {
            assert( m.isEmpty() );
            assert( sizeof(escCntlChar) == sizeof(escCntlCode) );

            const char * const esc = strchr(escCntlChar,c);
            if(0!=esc)
            {
                m = new Byte( escCntlCode[esc-escCntlChar] );
                return true;
            }
            else
            {
                return false;
            }
        }


        Pattern * RXC:: subExprEsc()
        {
            const char * const start = curr-1;
            assert('\\'==*start);

            if(curr>=last) throw Specific::Exception(CallSign, "unfinished escaped sequence in sub-expression");
            const char       c = *(curr++);
            AutoPtr<Pattern> m = 0;

            if(isExprChar(c,m)) { assert(m.isValid()); return m.yield(); }
            if(isCntlChar(c,m)) { assert(m.isValid()); return m.yield(); }

            throw Specific::Exception(CallSign, "unknow escape sequence '\\%c...'",c);
        }
    }

}


