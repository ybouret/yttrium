#include "y/information/entropic/unit.hpp"
#include "y/text/ascii/printable.hpp"
#include <iomanip>

namespace Yttrium
{

    namespace Information
    {

        namespace Entropic
        {

            Unit:: ~Unit() noexcept
            {
            }

            Unit:: Unit(const uint16_t userData, const uint16_t userBits) noexcept :
            next(0),
            prev(0),
            priv(0),
            freq(0),
            code(userData),
            bits(userBits),
            data(userData),
            nbit(userBits)
            {

            }

            const char * Unit:: name() const noexcept
            {
                if(data<256) return ASCII::Printable::Char[ uint8_t(data) ];
                if(Unit::EOS==data) return "EOS";
                if(Unit::NYT==data) return "NYT";
                return "???";
            }


            void Unit:: reset() noexcept
            {
                freq = 0;
                code = data;
                bits = nbit;
                prev = next = 0;
            }

            void Unit:: reduceFrequency() noexcept
            {
                switch(freq)
                {
                    case 0:
                    case 1:
                        break;
                    default:
                        freq >>= 1;
                }
            }

            void Unit:: to(StreamBits &io) const
            {
                io.push(code,bits);
            }
           
            std::ostream & Unit:: display(std::ostream &os) const
            {
                os << std::setw(5) << name();
                return os;
            }


        }

    }

}
