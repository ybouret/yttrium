
#include "y/lingo/pattern/regexp/compiler.hpp"
#include "y/lingo/pattern/all.hpp"
#include "y/system/exception.hpp"
#include "y/text/hexadecimal.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Lingo
    {
        static const char escCtlChar[] = "0abfnrtv";
        static const char escCtlCode[] = "\0\a\b\f\n\r\t\v";


        static inline bool isCtlChar(const char c, AutoPtr<Pattern> &m)
        {
            assert( m.isEmpty() );
            assert( sizeof(escCtlChar) == sizeof(escCtlCode) );

            const char * const esc = strchr(escCtlChar,c);
            if(0!=esc)
            {
                m = new Byte( escCtlCode[esc-escCtlChar] );
                return true;
            }
            else
            {
                return false;
            }
        }


        static inline bool isEscaped(const char c, const char * const s,  AutoPtr<Pattern> &m )
        {
            assert( m.isEmpty() );
            assert( 0 != s);

            if(0!=strchr(s,c))
            {
                m = new Byte(c);
                return true;
            }
            else
            {
                return false;
            }
        }
    }

}



namespace Yttrium
{
    namespace Lingo
    {
        
        Pattern    * RXC:: escapedSeq(const char * const seq)
        {
            const char * const start = curr-1;
            assert('\\'==*start);
            assert(0!=seq);

            if(curr>=last) throw Specific::Exception(CallSign, "unfinished escaped sequence");
            const char       c = *(curr++);
            switch(c)
            {
                case 'x': return hexEsc();
                default:
                    break;
            }

            AutoPtr<Pattern> m = 0;
            if(isEscaped(c,seq,m)) { assert(m.isValid()); return m.yield(); }
            if(isCtlChar(c,m))     { assert(m.isValid()); return m.yield(); }

            throw Specific::Exception(CallSign, "unknow escape sequence after '...%s'",start);
        }


        Pattern * RXC:: subExprEsc()
        {
            return escapedSeq("(){}[]+*?&\\/'\".");
        }


    }

}

namespace Yttrium
{
    namespace Lingo
    {


        Pattern * RXC:: subBankEsc()
        {
            return escapedSeq("[]-^\\/'\"");
        }



        Pattern    *RXC:: hexEsc()
        {
            assert('x' == curr[-1]);

            if(curr>=last) throw Specific::Exception(CallSign, "missing first hexadecimal char");
            const char  hiChar = *(curr++);
            const int   hiCode = Hexadecimal::ToDecimal(hiChar);
            if(hiCode<0) throw Specific::Exception(CallSign, "invalid first hexadecimal char '%c'", hiChar);

            if(curr>=last) throw Specific::Exception(CallSign, "missing second hexadecimal char");
            const char  loChar = *(curr++);
            const int   loCode = Hexadecimal::ToDecimal(loChar);
            if(loCode<0) throw Specific::Exception(CallSign, "invalid second hexadecimal char '%c'", loChar);


            return new Byte( static_cast<uint8_t>(hiCode<<4|loCode) );
        }
    }

}


