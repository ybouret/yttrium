#include "y/apex/block/blocks.hpp"

#include "y/utest/run.hpp"
#include "y/calculus/base2.hpp"
#include "y/memory/allocator/archon.hpp"
#include "y/system/exception.hpp"
#include "y/quantized.hpp"
#include "y/data/list/cxx.hpp"
#include "y/data/pool/cxx.hpp"
#include "y/type/proxy.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/type/utils.hpp"
#include "y/data/rework.hpp"
#include "y/sort/merge.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/calculus/bit-count.hpp"
#include "y/random/park-miller.hpp"

#include <cstring>

namespace Yttrium
{
    namespace Apex
    {

        
      

        class Factory : public Singleton<Factory>
        {
        public:
            static const char * const      CallSign;
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 27;
            static const unsigned PossibleShifts = Block::MaxShift-Block::MinShift+1;
            static const unsigned BytesPerBlocks = sizeof(Blocks);
            static const unsigned WorkspaceBytes = BytesPerBlocks * PossibleShifts;


            Block * acquire(const unsigned shift)
            {
                Y_LOCK(access);
                if(shift<=Block::MinShift) return blocks[Block::MinShift].query();
                if(shift>Block::MaxShift) throw Specific::Exception(CallSign,"shift overflow");
                return blocks[shift].query();
            }

            Block * acquireBytes(size_t bytes) {
                if(bytes>Block::MaxBytes) throw Specific::Exception(CallSign,"bytes overflow");
                return acquire( Base2<size_t>::LogFor(bytes) );
            }

            void release(Block * const block) noexcept {
                assert(block!=0);
                assert(block->shift>=Block::MinShift);
                assert(block->shift<=Block::MaxShift);
                blocks[block->shift].store(block);
            }

            void gc() noexcept {
                for(unsigned shift=Block::MinShift;shift<=Block::MaxShift;++shift)
                    blocks[shift].gc();
            }

            void display() const {
                std::cerr << callSign() << " content:" << std::endl;
                for(unsigned shift=Block::MinShift;shift<=Block::MaxShift;++shift)
                {
                    const Blocks &B = blocks[shift];
                    if(B->size<=0) continue;
                    std::cerr << "#2^" << std::setw(2) << shift << " : " << std::setw(8) << B->size << std::endl;
                }
            }

            Block * acquire(Random::Bits &ran, const size_t bits)
            {
                const size_t   bytes = Y_Apex_Bytes_For(bits);
                Block  * const block = acquireBytes(bytes);
                if(bytes>0)
                {
                    const size_t   msindx = bytes-1;
                    const unsigned msbits = static_cast<unsigned>(bits - (msindx<<3));
                    assert(msbits>=1);
                    assert(msbits<=8);

                    // formatting jig1
                    Jig1 &          jig1 = block->as<Plan1>();
                    uint8_t * const b    = jig1.word;
                    b[msindx] = ran.to<uint8_t>(msbits);
                    for(size_t i=0;i<msindx;++i)
                        b[i] = ran.to<uint8_t>();
                    Coerce(block->bits) = bits;
                    Coerce(jig1.words)  = bytes;


                    // sync other
                    assert(jig1.chkBits(bits));
                    block->syncDull();
                }
                return block;

            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Factory);
            friend class Singleton<Factory>;

            Blocks * const blocks;
            void   *       wksp[Y_WORDS_GEQ(WorkspaceBytes)];


            explicit Factory() noexcept :
            blocks(0),
            wksp()
            {
                Y_USHOW(Block::MinShift);
                Y_USHOW(Block::MaxShift);

                Y_USHOW(PossibleShifts);
                Y_USHOW(BytesPerBlocks);
                Y_USHOW(WorkspaceBytes);
                Y_SIZEOF(wksp);

                memset(wksp,0,sizeof(wksp));
                Coerce(blocks) = static_cast<Blocks *>( Memory::OutOfReach::Addr(wksp) ) - Block::MinShift;
                for(unsigned shift=Block::MinShift;shift<=Block::MaxShift;++shift)
                {
                    new ( &blocks[shift] ) Blocks(shift);
                }
            }

            virtual ~Factory() noexcept
            {
                for(unsigned shift=Block::MaxShift;shift>=Block::MinShift;--shift)
                    Destruct( &blocks[shift] );
                Coerce(blocks) = 0;
                memset(wksp,0,sizeof(wksp));
            }
        };

        const char * const Factory:: CallSign = "Apex::Factory";

    }
}

using namespace Yttrium;
using namespace Apex;


Y_UTEST(apex_types)
{

    Random::ParkMiller ran;
    Y_SIZEOF(Apex::Block);
    Y_SIZEOF(Apex::Blocks);

    Y_SIZEOF(Jig1);
    Y_SIZEOF(Jig2);
    Y_SIZEOF(Jig4);
    Y_SIZEOF(Jig8);
    Y_SIZEOF(Jigs);


    Apex::Factory & F = Apex::Factory::Instance();

    {
        Block::List b;
        for(size_t iter=0;iter<20;++iter)
        {
            b.pushTail( F.acquire( ran.in(0,10) ) );
        }
        while(b.size) F.release(b.popTail());
    }

    F.display();
    F.gc();
    F.display();
    F.gc();
    F.display();

    Apex::Block b(0);
#if 0
    const size_t maxBits = b.range * 8;
    std::cerr << "maxBits=" << maxBits << std::endl;
    for(size_t i=0;i<=maxBits;++i)
    {
        std::cerr << "bits=" << std::setw(4) << i;
        for(size_t j=0;j<Apex::JigAPI::Plans;++j)
        {
            Apex::JigAPI &jig = b[ Apex::Plan(j) ];
            jig.updateFor(i);
            std::cerr << " | " << jig.words;
        }

        std::cerr << std::endl;
    }
#endif

    std::cerr << "Plan1" << std::endl;
    b.as<Plan1>().word[0] = 0x12;
    b.as<Plan1>().word[1] = 0x0a;
    b.as<Plan1>().word[2] = 0xe0;
    b.sync();
    std::cerr << "b="    << b      << std::endl;
    std::cerr << "bits=" << b.bits << std::endl;


    std::cerr << "Plan2" << std::endl;
    b.to(Plan2);
    std::cerr << "b=" << b << std::endl;
    std::cerr << "bits=" << b.bits << std::endl;

    std::cerr << "Plan1" << std::endl;
    b.to(Plan1);
    std::cerr << "b=" << b << std::endl;
    std::cerr << "bits=" << b.bits << std::endl;



    for(unsigned nbit=0;nbit<=10;++nbit)
    {
        Apex::Block * p = F.acquire(ran,nbit);
        std::cerr << *p << std::endl;
        F.release(p);
    }
}
Y_UDONE()


