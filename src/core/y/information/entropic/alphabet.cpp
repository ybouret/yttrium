
#include "y/information/entropic/alphabet.hpp"

namespace Yttrium
{

    namespace Information
    {

        namespace Entropic
        {
            Unit::  Unit(const Code c, const Bits b) noexcept : code(c), bits(b), freq(0), next(0), prev(0) {}
            Unit:: ~Unit() noexcept {}
        }

        namespace Entropic
        {
            Alphabet:: Alphabet() noexcept :
            unit(0),
            emit( & Alphabet::emitInit ),
            eos(0),
            nyt(0),
            used(),
            wksp()
            {
                Coerce(unit) = static_cast<Unit *>(Y_STATIC_ZARR(wksp));
                for(Code code=0;code<Bytes;++code)
                {
                    new (unit+code) Unit(code,8);
                }
                for(Code code=Bytes;code<Units;++code)
                {
                    new (unit+code) Unit(code,0);
                }
                Coerce(eos) = unit + EOS;
                Coerce(nyt) = unit + NYT;
                pushControls();
            }


            Alphabet:: ~Alphabet() noexcept
            {
                for(Code code=0;code<Units;++code)
                    Memory::OutOfReach::Naught(unit+code);
                used.reset();
            }

            void Alphabet:: send(StreamBits &io, const Unit &u)
            {
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
                io.push(eos->code,eos->bits);
                io.fill();
            }

            void Alphabet:: pushControls() noexcept
            {
                used.pushTail(eos);
                used.pushTail(nyt);
            }

            void Alphabet:: reset() noexcept
            {

                used.reset();

                for(Code code=0;code<Bytes;++code)
                {
                    Unit &u = unit[code];
                    u.freq = 0;
                    u.bits = 8;
                    u.next = 0;
                    u.prev = 0;
                }

                for(Code code=Bytes;code<Units;++code)
                {
                    Unit &u = unit[code];
                    assert(0==u.freq);
                    assert(0==u.bits);
                    u.next = 0;
                    u.prev = 0;
                }

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

                send(io,u);
                used.pushHead(&u)->freq++;
                emit = & Alphabet::emitBulk;
            }

            void Alphabet:: emitBulk(StreamBits &io, Unit &u)
            {
                assert(used.owns(eos));
                assert(used.owns(nyt));
                assert(used.size<Units);
                if(u.freq++ <= 0)
                {
                    //----------------------------------------------------------
                    // a new unit
                    //----------------------------------------------------------
                    assert( !used.owns(&u) );
                    assert( 8 == u.bits    );

                    send(io,*nyt);             // feed Not Yet Transmitted
                    used.insertBefore(eos,&u); // put into position
                    if(used.size>=Units)
                    {
                        used.pop(nyt);
                        emit = & Alphabet:: emitFull;
                    }
                }
                else
                    rank(u);

                send(io,u);
            }

            void Alphabet:: emitFull(StreamBits &io, Unit &u)
            {
                assert(1+Bytes==used.size);
                assert(u.freq>0);
                send(io,u);
                ++u.freq;
                rank(u);
            }



        }

    }

}
