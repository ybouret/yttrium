
#include "y/utest/run.hpp"
#include "y/calculus/align.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/data/list/raw.hpp"
#include "y/stream/bits.hpp"

namespace Yttrium
{

    namespace Information
    {
     
        typedef uint32_t Frequency;
        typedef uint16_t Code;
        typedef uint8_t  Bits;

        class Unit
        {
        public:
            typedef RawListOf<Unit> List;
            explicit Unit(const Code c, const Bits b) noexcept : code(c), bits(b), freq(0), next(0), prev(0) {}
             inline ~Unit() noexcept {}

            const Code code;
            Bits       bits;
            Frequency  freq;
            Unit      *next;
            Unit      *prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Unit);
        };

        enum CtrlOffset
        {
            CtrlEOS = 0,
            CtrlNYT = 1
        };


        class Alphabet
        {
        public:
            static const Code     Bytes    = 256;
            static const Code     Ctrls    = 2;
            static const Code     Units    = Bytes + Ctrls;
            static const Code     EOS      = Bytes + CtrlEOS;
            static const Code     NYT      = Bytes + CtrlNYT;
            static const unsigned Required = Units * sizeof(Unit);

            typedef void (Alphabet::*Emit)(StreamBits &, Unit &);

            explicit Alphabet() :
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

            virtual ~Alphabet() noexcept
            {
                for(Code code=0;code<Units;++code)
                    Memory::OutOfReach::Naught(unit+code);
                used.reset();
            }

            void write(const uint8_t byte,
                       StreamBits   &io)
            {
                Unit &u = unit[byte];
            }



        protected:
            Unit       * const unit;
            Emit               emit;
            Unit       * const eos;
            Unit       * const nyt;
            Unit::List         used;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Alphabet);

            void *wksp[ Y_WORDS_GEQ(Required) ];

            inline void feed(StreamBits &io, const Unit &u)
            {
                io.push(u.code,u.bits);
            }


            //! emit first unit
            void emitInit(StreamBits &io, Unit &u)
            {
                assert(Ctrls==used.size);
                assert(used.owns(eos));
                assert(used.owns(nyt));
                assert(!used.owns(&u));
                assert(0==u.freq);
                assert(8==u.bits);

                feed(io,u);
                used.pushHead(&u)->freq++;
                emit = & Alphabet::emitBulk;
            }

            void emitBulk(StreamBits &io, Unit &u)
            {
                assert(used.owns(eos));
                assert(used.owns(nyt));
                assert(used.size<Units);
                if(u.freq++ <= 0)
                {
                    // a new unit
                    assert( !used.owns(&u) );
                    assert( 8 == u.bits    );


                    feed(io,*nyt);             // feed Not Yet Transmitted
                    used.insertBefore(eos,&u); // put into position
                }
                else
                {
                    while(u.prev && u.prev->freq < u.freq )
                    {

                    }
                }

            }

            void pushControls() noexcept
            {
                used.pushTail(eos);
                used.pushTail(nyt);
            }

            void reset() noexcept
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

        };


    }

}

using namespace Yttrium;


Y_UTEST(info_pack)
{
    Y_SIZEOF(Information::Unit);
    Y_USHOW(Information::Alphabet::Required);
    Information::Alphabet alpha;

}
Y_UDONE()
