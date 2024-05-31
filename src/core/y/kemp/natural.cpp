#include "y/kemp/natural.hpp"
#include "y/kemp/element.hpp"
#include "y/type/nullify.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        Natural:: ~Natural() noexcept
        {
            assert(0!=code);
            Nullify(code);
        }

        Natural:: Natural() : code( new Element(0,AsCapacity) )
        {
        }

        Natural:: Natural(const Natural &n) : code( new Element( *n.code) ) {}

        void Natural:: xch(Natural &n) noexcept
        {
            Swap(code,n.code);
        }

        Natural & Natural:: operator=(const Natural &n)
        {
            {Natural _(n); xch(_);}
            return *this;
        }


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
                while(i>0)
                {
                    const uint8_t tmp = b.item[--i];
                    s << Hexadecimal::Text[tmp];
                }
            }
            return       s;

        }
        


    }

}


