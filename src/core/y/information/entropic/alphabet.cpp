
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



            Alphabet:: Alphabet(const Mode mode) noexcept :
            symbols(),
            active(0),
            symbol(0),
            nyt(0),
            eos(0),
            control(),
            controls(0),
            workspace()
            {

                Coerce(symbol) = static_cast<Symbol *>(Y_STATIC_ZARR(workspace));

                for(uint16_t i=0;i<Symbol::Encoding;++i)
                {
                    new (symbol+i) Symbol(i,8);
                }

                for(uint16_t i=Symbol::Encoding;i<Symbol::Universe;++i)
                {
                    new (symbol+i) Symbol(i,0);
                }

                Coerce(nyt) = &symbol[ Symbol::NYT ];
                Coerce(eos) = &symbol[ Symbol::EOS ];

                initControls(mode);
            }

            void Alphabet:: initControls(const Mode mode) noexcept
            {
                memset(control,0,sizeof(control));
                controls = 0;
                switch(mode)
                {
                    case Precompiling:
                        break;

                    case SingleStream:
                        control[controls++] = nyt;
                        break;

                    case MultiStreams:
                        control[controls++] = nyt;
                        control[controls++] = eos;
                        break;
                }
                pushControls();
            }

            void Alphabet:: pushControls() noexcept
            {
                for(size_t i=0;i<controls;++i) symbols.pushTail(control[i]);
            }


            Alphabet:: ~Alphabet() noexcept
            {
                symbols.reset();
            }

            void Alphabet:: reset() noexcept
            {
                symbols.reset();
                active = 0;
                for(uint16_t i=0;i<Symbol::Universe;++i)
                    symbol[i].reset();
                pushControls();
            }


            void Alphabet:: reduceFrequencies() noexcept
            {
                for(Symbol *symb=symbols.head;symb;symb=symb->next)
                    symb->reduceFrequency();
            }


            void Alphabet:: display(std::ostream &os) const
            {
                os << "<Alphabet controls='" << controls << "'>" << std::endl;
                for(const Symbol *symb=symbols.head;symb;symb=symb->next)
                {
                    symb->display(os << "  ") << std::endl;
                }
                os << "<Alphabet>" << std::endl;
            }


            const Symbol & Alphabet:: NYT() const noexcept
            {
                assert(0!=nyt);
                return *nyt;
            }

            const Symbol & Alphabet:: EOS() const noexcept
            {
                assert(0!=eos);
                return *eos;
            }


        }

    }

}
