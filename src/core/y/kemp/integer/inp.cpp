#include "y/kemp/integer.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        Natural StringToDec(const char * const s,
                            const size_t       n);

        Integer:: Integer(const String &str) :
        Number(), s(Positive), n()
        {
            const char *ptr = str.c_str();
            size_t      num = str.size();

            if(num<=0) throw Specific::Exception(CallSign, "empty string");

            // check sign
            {
                const char c = ptr[0];
                switch(c)
                {
                    case '-': Coerce(s) = Negative; break;
                    case '+': assert(Positive==s);  break;
                    default:
                        goto PARSE_N;
                }
                ++ptr;
                --num;
                if(num<=0) throw Specific::Exception(CallSign, "empty string after '%c'", c);
            }

            // parse integer part
        PARSE_N:
            Coerce(n) = StringToDec(ptr,num);
            chk();
        }

    }

}

