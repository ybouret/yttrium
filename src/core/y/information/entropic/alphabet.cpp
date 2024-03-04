
#include "y/information/entropic/alphabet.hpp"
#include "y/text/ascii/printable.hpp"
#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{

    namespace Information
    {



        namespace Entropic
        {

#define Y_ALPHA_MSG(MSG) do { if (verbose) { std::cerr << MSG << std::endl; } } while(false)

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
                for(Code code=0;code<Bytes;++code)
                {
                    Unit &u = unit[code];
                    u.code  = code;
                    u.bits  = 8;
                }

                Coerce(eos) = unit + EOS;
                Coerce(nyt) = unit + NYT;
                pushControls();
            }


            Alphabet:: ~Alphabet() noexcept
            {
                reset();
            }


            const char * Alphabet:: uid(const Unit &u) const noexcept
            {
                const size_t delta = &u - unit;
                if( delta < 256  ) return ASCII::Printable::Char[ uint8_t(delta) ];
                if( delta == EOS ) return "EOS";
                if( delta == NYT ) return "NYT";
                return Core::Unknown;
            }

            void Alphabet:: send(StreamBits &io, const Unit &u)
            {
                Y_ALPHA_MSG("--> '" << uid(u) << "'");
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

                used.reset(); Y_STATIC_ZARR(wksp);

                for(Code code=0;code<Bytes;++code)
                {
                    Unit &u = unit[code];
                    u.code = code;
                    u.bits = 8;
                }

                sumf = 0;
                emit = & Alphabet::emitInit;
                pushControls();
            }


            void Alphabet:: emitInit(StreamBits &io, Unit &u)
            {
                assert(Ctrls==used.size);
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
                assert(used.owns(eos));
                assert(used.owns(nyt));
                assert(used.size<Units);
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
                    if(used.size>=Units)
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
                assert(1+Bytes==used.size);
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
                while(sumf>=MaxSumFreq)
                    reduce();
            }


            void Alphabet:: write(StreamBits &io, const uint8_t byte)
            {
                write_(io,byte);
            }
        }

    }

}
