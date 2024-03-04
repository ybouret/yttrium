
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

            static inline const char * UnitUID(const unsigned byte)
            {
                if(byte<256) return ASCII::Printable::Char[uint8_t(byte)];
                if(Unit::EOS == byte) return "EOS";
                if(Unit::NYT == byte) return "NYT";
                return "???";
            }

            std::ostream & operator<<(std::ostream &os, const Unit &unit)
            {
                StreamBits io;
                io.push(unit.code,unit.bits);
                os 
                << std::setw(5) << UnitUID(unit.byte)
                << " = |" << io << "|=" << unit.bits << " @freq=" << unit.freq;
                return os;
            }

#define Y_ALPHA_MSG(MSG) do { if (verbose) { std::cerr << MSG << std::endl; } } while(false)


            void Alphabet:: init() noexcept
            {
                for(unsigned code=0;code<Unit::Encoding;++code)
                {
                    Unit &u = unit[code];
                    u.code  = u.byte = code;
                    u.bits  = 8;
                }

                for(unsigned code=Unit::Encoding;code<Unit::Universe;++code)
                {
                    unit[code].byte = code;
                }

            }

            Alphabet:: Alphabet(const OperatingMode how,
                                const bool          verbosity) noexcept :
            unit(0),
            emit( & Alphabet::emitInit ),
            eos(0),
            nyt(0),
            used(),
            sumf(0),
            mode(how),
            verbose(verbosity),
            wksp()
            {
                Coerce(unit) = static_cast<Unit *>(Y_STATIC_ZARR(wksp));
                init();
                Coerce(eos) = unit + Unit::EOS;
                Coerce(nyt) = unit + Unit::NYT;
                pushControls();
            }


            Alphabet:: ~Alphabet() noexcept
            {
                reset();
            }




            void Alphabet:: send(StreamBits &io, const Unit &u)
            {
                Y_ALPHA_MSG("--> '" << UnitUID(u.byte) << "'");
                io.push(u.code,u.bits);
            }

            void Alphabet:: rank(Unit &u) noexcept
            {
                assert(used.owns(&u));
                assert(u.freq>0);
                while(u.prev && u.prev->freq < u.freq )
                    used.towardsHead(&u);

            }

            void Alphabet:: flush(StreamBits &io)
            {
                switch(mode)
                {
                    case Multiplex:
                        Y_ALPHA_MSG("flush Multiplex");
                        io.push(eos->code,eos->bits);
                        break;

                    case BlockWise:
                        Y_ALPHA_MSG("flush BlockWise");
                        assert(!used.owns(eos));
                        break;
                }

                io.fill();
            }

            void Alphabet:: pushControls() noexcept
            {
                switch(mode)
                {
                    case Multiplex:
                        used.pushTail(eos);
                        break;

                    case BlockWise:
                        break;
                }

                used.pushTail(nyt);
            }


            void Alphabet:: reset() noexcept
            {

                used.reset(); 
                Y_STATIC_ZARR(wksp);
                init();
                sumf = 0;
                emit = & Alphabet::emitInit;
                pushControls();
            }


            void Alphabet:: emitInit(StreamBits &io, Unit &u)
            {
                assert(Unit::Controls==used.size);
                assert(used.owns(eos));
                assert(used.owns(nyt));
                assert(!used.owns(&u));
                assert(0==u.freq);
                assert(8==u.bits);

                Y_ALPHA_MSG("emitInit");
                send(io,u);
                used.pushHead(&u)->freq++;
                sumf++;
                emit = & Alphabet::emitBulk;
            }

            void Alphabet:: emitBulk(StreamBits &io, Unit &u)
            {
                assert(used.owns(nyt));
                sumf++;
                if(u.freq++ <= 0)
                {
                    //----------------------------------------------------------
                    // a new unit
                    //----------------------------------------------------------
                    assert( !used.owns(&u) );
                    assert( 8 == u.bits    );

                    Y_ALPHA_MSG("emitBulk NYT");

                    send(io,*nyt);             // feed Not Yet Transmitted
                    used.insertBefore(eos,&u); // put into position
                    if(used.size>=Unit::Universe)
                    {
                        Y_ALPHA_MSG("switch to FULL");
                        used.pop(nyt);
                        emit = & Alphabet:: emitFull;
                    }
                }
                else
                {
                    //----------------------------------------------------------
                    // an existing unit: ranking
                    //----------------------------------------------------------
                    Y_ALPHA_MSG("emitBULK USED");
                    rank(u);
                }

                send(io,u);
            }

            void Alphabet:: emitFull(StreamBits &io, Unit &u)
            {
                assert(u.freq>0);
                Y_ALPHA_MSG("emit USED Full(" << char(u.code) << ")");
                send(io,u);
                u.freq++;
                sumf++;
                rank(u);
            }

         
            void Alphabet:: reduce() noexcept
            {
                Y_ALPHA_MSG("reducing frequencies");
                sumf = 0;
                for(Unit *u=used.head;u;u=u->next)
                {
                    Frequency &freq = u->freq;
                    switch(freq)
                    {
                        case 0:    // unchanged
                        case 1:    // unchanged
                            break;
                        default:
                            freq >>= 1;
                            if(freq<=0) freq = 1;
                    }
                    sumf += freq;
                }
            }


            void Alphabet:: write_(StreamBits &io, const uint8_t byte)
            {
                ( (*this).*emit )(io,unit[byte]);
                assert(used.size<=257);
                while(sumf>=Unit::MaxSumFreq)
                    reduce();
            }


            void Alphabet:: write(StreamBits &io, const uint8_t byte)
            {
                write_(io,byte);
            }

            void Alphabet:: write(StreamBits &io, const uint8_t byte, Model &model)
            {
                write_(io,byte);
                model.build(used);
            }

            void Alphabet:: display(std::ostream &os) const
            {
                os << "<Alphabet used='" << used.size << "'>" << std::endl;
                for(const Unit *u=used.head;u;u=u->next)
                {
                    os << *u << std::endl;
                }

                os << "<Alphabet/>" << std::endl;
            }
        }

    }

}
