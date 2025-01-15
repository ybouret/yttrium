
#include "y/apex/block/factory.hpp"
#include "y/system/exception.hpp"
#include <iomanip>
#include <cstring>

namespace Yttrium
{
    namespace Apex
    {

        const char * const Factory:: CallSign = "Apex::Factory";

        Factory:: Factory() noexcept :
        blocks(0),
        mutexes(CallSign),
        wksp()
        {

            memset(wksp,0,sizeof(wksp));
            Coerce(blocks) = static_cast<Blocks *>( Memory::OutOfReach::Addr(wksp) ) - Block::MinShift;
            for(unsigned shift=Block::MinShift;shift<=Block::MaxShift;++shift)
            {
                new ( &blocks[shift] ) Blocks(shift);
            }
        }

        Factory:: ~Factory() noexcept
        {
            for(unsigned shift=Block::MaxShift;shift>=Block::MinShift;--shift)
                Destruct( &blocks[shift] );
            Coerce(blocks) = 0;
            memset(wksp,0,sizeof(wksp));
        }


        Block * Factory:: query(const unsigned shift)
        {
            Y_LOCK(access);
            if(shift<=Block::MinShift) return blocks[Block::MinShift].query();
            if(shift>Block::MaxShift)  throw  Specific::Exception(CallSign,"shift overflow");
            return blocks[shift].query();
        }

        Block * Factory:: duplicate(const Block * const other)
        {
            return queryBytes(other->bytes)->duplicate(other);
        }



        Block * Factory:: queryBytes(size_t bytes) {
            if(bytes>Block::MaxBytes) throw Specific::Exception(CallSign,"bytes overflow");
            return query( Base2<size_t>::LogFor(bytes) );
        }


        void Factory:: store(Block * const block) noexcept {
            assert(block!=0);
            assert(block->shift>=Block::MinShift);
            assert(block->shift<=Block::MaxShift);
            blocks[block->shift].store(block);
        }

        void Factory:: gc(const size_t cycles) noexcept {
            for(unsigned shift=Block::MinShift;shift<=Block::MaxShift;++shift)
                blocks[shift].gc(cycles);
            mutexes.gc(cycles);
        }

        void Factory:: display() const {
            std::cerr << callSign() << " content:" << std::endl;
            for(unsigned shift=Block::MinShift;shift<=Block::MaxShift;++shift)
            {
                const Blocks &B = blocks[shift];
                if(B->size<=0) continue;
                std::cerr << "#2^" << std::setw(2) << shift << " : " << std::setw(8) << B->size << std::endl;
            }
        }

        Block * Factory:: query(Random::Bits &ran, const size_t bits)
        {
            const size_t   bytes = Y_Apex_Bytes_For(bits);
            Block  * const block = queryBytes(bytes);
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

        Mutex * Factory:: query()
        {
            return mutexes.query();
        }

        void Factory:: store(Mutex * const mtx) noexcept
        {
            assert(0!=mtx);
            mutexes.store(mtx);
        }

    }

}

