
#include "y/information/entropic/alphabet.hpp"
#include "y/text/ascii/printable.hpp"
#include "y/memory/out-of-reach.hpp"
#include <iomanip>

namespace Yttrium
{

    namespace Information
    {

        namespace Entropic
        {
            const char * Alphabet:: ModeText(const Mode m) noexcept
            {
                switch(m)
                {
                    case Precompiling: return "Precompiling";
                    case SingleStream: return "SingleStream";
                    case MultiStreams: return "MultiStreams";
                }
                return Core::Unknown;
            }

            const char * Alphabet:: modeText()   const noexcept
            {
                return ModeText(mode);
            }



            Alphabet:: Alphabet(const Mode m) noexcept :
            symbols(),
            sumFreq(0),
            active(0),
            symbol(0),
            mode(m),
            nyt(0),
            eos(0),
            workspace()
            {

                Coerce(symbol) = static_cast<Symbol *>(Y_STATIC_ZARR(workspace));

                for(uint16_t i=0;i<Symbol::Encoding;++i)                new (symbol+i) Symbol(i,8);
                for(uint16_t i=Symbol::Encoding;i<Symbol::Universe;++i) new (symbol+i) Symbol(i,0);

                Coerce(nyt) = &symbol[ Symbol::NYT ];
                Coerce(eos) = &symbol[ Symbol::EOS ];

                pushControls();
            }



            void Alphabet:: pushControls() noexcept
            {
                switch(mode)
                {
                    case Precompiling:
                        break;

                    case SingleStream:
                        symbols.pushTail(nyt);
                        break;

                    case MultiStreams:
                        symbols.pushTail(eos);
                        symbols.pushTail(nyt);
                        break;
                }
            }


            Alphabet:: ~Alphabet() noexcept
            {
                symbols.reset();
            }

            void Alphabet:: reset() noexcept
            {
                symbols.reset();
                sumFreq = 0;
                active  = 0;
                for(uint16_t i=0;i<Symbol::Universe;++i)
                    symbol[i].reset();
                pushControls();
            }


            void Alphabet:: reduceFrequencies() noexcept
            {
                sumFreq = 0;
                for(Symbol *symb=symbols.head;symb;symb=symb->next)
                    sumFreq += symb->reduceFrequency();
            }


            void Alphabet:: display(std::ostream &os) const
            {
                os << "<Alphabet mode='" << modeText() << "' sumFreq='" << sumFreq << "'>" << std::endl;
                for(const Symbol *symb=symbols.head;symb;symb=symb->next)
                {
                    symb->display(os << "  ") << std::endl;
                }
                os << "<Alphabet>" << std::endl;
            }

            const Symbol & Alphabet:: operator[](const uint8_t byte) const noexcept
            {
                assert(0!=symbol);
                return symbol[byte];
            }

            



        }

    }

}
