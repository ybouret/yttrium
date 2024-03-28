
#include "y/information/entropic/alphabet.hpp"
#include "y/text/ascii/printable.hpp"
#include "y/memory/out-of-reach.hpp"
#include <iomanip>
#include "y/system/exception.hpp"

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
                        symbols.pushTail(eos);
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

            void Alphabet:: insertSymbol(Symbol * const node) noexcept
            {
                assert(0!=node);
                assert(1==node->freq);
                assert(active<Symbol::Encoding);
                std::cerr << "new symbol '" << node->name() << "'" << std::endl;
                switch(mode)
                {
                    case Precompiling:
                        assert(false == symbols.owns(eos));
                        assert(false == symbols.owns(nyt));
                        symbols.pushTail(node);
                        break;

                    case SingleStream:
                        assert(true  == symbols.owns(eos));
                        assert(false == symbols.owns(nyt));
                        assert(symbols.tail==eos);
                        symbols.insertBefore(eos,node);
                        break;

                    case MultiStreams:
                        assert(true  == symbols.owns(eos));
                        switch(active)
                        {
                            case Symbol::Encoding:
                                assert(false == symbols.owns(nyt));
                                symbols.insertBefore(eos,node);
                                break;

                            case Symbol::Encoding-1:
                                assert(true == symbols.owns(nyt));
                                assert(nyt == symbols.tail);
                                (void) symbols.popTail();
                                symbols.insertBefore(eos,node);
                                break;

                            default:
                                assert(active<Symbol::Encoding-1);
                                assert(true == symbols.owns(nyt));
                                symbols.insertBefore(eos,node);
                                break;
                        }
                        break;
                }
                ++active;
            }

            void Alphabet:: updateSymbol(Symbol * const mine) noexcept
            {
                assert(mine->freq>1);
                std::cerr << "upd symbol '" << mine->name() << "'" << std::endl;
                const Symbol::FreqType freq = mine->freq;
                while( (0 != mine->prev) && (mine->prev->freq<freq) ) symbols.towardsHead(mine);
            }

            void Alphabet:: monitorFreqs() noexcept
            {
                if(++sumFreq>=Symbol::MaxFreq)
                    reduceFrequencies();
            }


            void Alphabet:: update(const Symbol &symb) noexcept
            {
                Symbol * const mine = & Coerce(symb);

                if(mine->freq++ <= 0)
                    insertSymbol(mine);
                else
                    updateSymbol(mine);

                monitorFreqs();
            }


            void Alphabet:: commit(Model &model) noexcept
            {
                assert(symbols.size>0);
                model.build(symbols);
            }


            size_t Alphabet:: outputBytesFor(const Memory::ReadOnlyBuffer &buffer) const
            {
                size_t         bits = 0;
                const uint8_t *data = static_cast<const uint8_t *>(buffer.ro_addr());
                for(size_t i=buffer.measure();i>0;--i)
                {
                    const uint8_t byte = *(data++);
                    const Symbol &symb = symbol[byte];
                    if(symb.freq<=0) throw  Exception("unregistered '%s' from buffer in Alphabet::outputBytesFor", symb.name());
                    bits += symb.bits;
                }
                return Y_ALIGN8(bits) >> 3;
            }


        }

    }

}
