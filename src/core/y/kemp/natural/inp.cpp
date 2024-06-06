#include "y/kemp/natural.hpp"
#include "y/kemp/element.hpp"
#include "y/type/nullify.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        static inline Natural StringToHex(const char * const s,
                                          const size_t       n)
        {
            static const char fn[] = "String To Hexadecimal";
            if(n<=0) throw Specific::Exception(Natural::CallSign, "%s on Empty String", fn);

            Natural res = 0;
            for(size_t i=0;i<n;++i)
            {
                res <<= 4;
                const char c = s[i];
                switch(c)
                {
                    case '0': break;
                    case '1': res += 1; break;
                    case '2': res += 2; break;
                    case '3': res += 3; break;
                    case '4': res += 4; break;
                    case '5': res += 5; break;
                    case '6': res += 6; break;
                    case '7': res += 7; break;
                    case '8': res += 8; break;
                    case '9': res += 9; break;
                    case 'a':
                    case 'A': res += 10; break;
                    case 'b':
                    case 'B': res += 11; break;
                    case 'c':
                    case 'C': res += 12; break;
                    case 'd':
                    case 'D': res += 13; break;
                    case 'e':
                    case 'E': res += 14; break;
                    case 'f':
                    case 'F': res += 15; break;
                    default:
                        throw Specific::Exception(Natural::CallSign, "%s invalid '%c'", fn, c);
                }
            }
            return res;
        }

        static inline Natural StringToDec(const char * const s,
                                          const size_t       n)
        {
            static const char fn[] = "String To Decimal";
            assert(0!=s);
            assert(n>0);

            const Natural ten = 10;
            Natural       res = 0;
            for(size_t i=0;i<n;++i)
            {
                res *= 10;
                const char c = s[i];
                switch(c)
                {
                    case '0': break;
                    case '1': res += 1; break;
                    case '2': res += 2; break;
                    case '3': res += 3; break;
                    case '4': res += 4; break;
                    case '5': res += 5; break;
                    case '6': res += 6; break;
                    case '7': res += 7; break;
                    case '8': res += 8; break;
                    case '9': res += 9; break;
                    default:
                        throw Specific::Exception(Natural::CallSign, "%s invalid '%c'", fn, c);
                }
            }
            return res;
        }



        static inline Natural StringToNatural(const char  * const s,
                                              const size_t        n)
        {
            assert(0!=s);
            if(n<=0) throw Specific::Exception(Natural::CallSign, "empty string to convert");

            if(n>=2 && '0' == s[0] && 'x' == s[1])
            {
                return StringToHex(s+2,n-2);
            }
            else
            {
                return StringToDec(s,n);
            }
        }

        Natural:: Natural(const String &str) :
        Number(),
        code( new Element(0,AsCapacity) )
        {
            try {
                Natural _ = StringToNatural(str.c_str(),str.size());
                xch(_);
            }
            catch(...)
            {
                Nullify(code);
                throw;
            }
        }
    }

}

