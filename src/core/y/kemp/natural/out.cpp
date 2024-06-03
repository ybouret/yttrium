
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


        String Natural:: toDec() const
        {
            String s;
            
            if(code->bits<=0)
            {
                s << '0';
            }
            else
            {
                const Natural ten = 10;
                Natural       self = *this;
                Natural       q,r;
                do
                {
                    Div(q,r,self,ten);
                    assert(r<10);
                    assert(r.code->u64()<10);               // will use r
                    s << Hexadecimal::Lower[r.code->u64()]; // use existing chars
                    self.xch(q);                            // update
                } while(self.code->bits>0);
                s.reverse();
            }
            return s;
        }

        std::ostream & operator<<(std::ostream &os, const Natural &n)
        {
            if( os.flags() & std::ios_base::hex )
            {
                os << n.toHex();
            }
            else
            {
                os << n.toDec();
            }


            return os;
        }
    }

}
