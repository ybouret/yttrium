#include "y/information/entropic/symbol.hpp"
#include "y/text/ascii/printable.hpp"
#include <iomanip>

namespace Yttrium
{

    namespace Information
    {

        namespace Entropic
        {

            Symbol:: Symbol(const uint16_t initCode,
                            const uint16_t initBits) noexcept :
            next(0),
            prev(0),
            data(0),
            freq(0),
            code(initCode),
            bits(initBits),
            _code(initCode),
            _bits(initBits)
            {
            }

            Symbol:: ~Symbol() noexcept
            {
            }


            void Symbol:: reset() noexcept
            {
                freq = 0;
                code = _code;
                bits = _bits;
                data = 0;
                next = 0;
                prev = 0;
            }

            void Symbol:: reduceFrequency() noexcept
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

            const char * Symbol:: name() const noexcept
            {
                if(_code<256) return ASCII::Printable::Char[ uint8_t(_code) ];
                if(EOS==_code) return "EOS";
                if(NYT==_code) return "NYT";
                return "???";
            }

            void Symbol:: to(StreamBits &io) const
            {
                io.push(code,bits);
            }

            std::ostream & Symbol:: display(std::ostream &os) const
            {
                os << std::setw(5) << name();
                StreamBits io;
                to(io);
                os << " : |" << std::setw(16) << io << "|=" << std::setw(2) << bits;
                os << " #"  << freq;
                return os;
            }
        }

    }

}
