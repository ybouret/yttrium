
#include "y/utest/run.hpp"
#include "y/calculus/align.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/data/list/raw.hpp"

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
            explicit Unit(const Code c, const Bits b) noexcept :
            code(c), bits(b), freq(0), next(0), prev(0){}
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
            static const Code     Bytes = 256;
            static const Code     Ctrls = 2;
            static const Code     Units = Bytes+Ctrls;
            static const Code     EOS   = Bytes + CtrlEOS;
            static const Code     NYT   = Bytes + CtrlNYT;
            static const unsigned Required = Units * sizeof(Unit);

            explicit Alphabet() :
            unit(0),
            used(),
            wksp()
            {
                unit = static_cast<Unit *>(Y_STATIC_ZARR(wksp));
                for(Code code=0;code<Bytes;++code)
                {
                    new (unit+code) Unit(code,8);
                }
                for(Code code=Bytes;code<Units;++code)
                {
                    new (unit+code) Unit(code,0);
                }
            }

            virtual ~Alphabet() noexcept
            {
                for(Code code=0;code<Units;++code)
                    Memory::OutOfReach::Naught(unit+code);
                used.reset();
            }
            


        protected:
            Unit       *unit;
            Unit::List  used;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Alphabet);

            void *wksp[ Y_WORDS_GEQ(Required) ];

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
