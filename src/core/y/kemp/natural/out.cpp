
#include "y/kemp/natural.hpp"
#include "y/kemp/element.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        String Natural:: toHex() const
        {
            String       s;
            const Bytes &b = code->get<uint8_t>();
            size_t       i = b.positive;
            if(i<=0)
                s << '0';
            else
            {
                const uint8_t msb = b.item[--i]; assert(msb>0);
                if(0!=(msb&0xf0) ) s << Hexadecimal::Text[msb]; else s << Hexadecimal::Lower[msb&0xff];
                while(i>0)
                {
                    const uint8_t tmp = b.item[--i];
                    s << Hexadecimal::Text[tmp];
                }
            }
            return s;
        }

        std::ostream & operator<<(std::ostream &os, const Natural &n)
        {
            os << n.toHex();
            return os;
        }
    }

}
