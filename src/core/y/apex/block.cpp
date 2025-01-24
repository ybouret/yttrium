#include "y/apex/block.hpp"
#include "y/check/static.hpp"
#include "y/check/crc32.hpp"
#include "y/calculus/byte-count.hpp"
#include <cstring>



namespace Yttrium
{
    namespace Apex
    {
        

        Block:: Block(const unsigned _shift) :
        DataBlock(_shift),
        Jigs(entry,range),
        bytes(as<Plan1>().words),
        next(0),
        prev(0),
        nref(0)
        {
            relink();
            assert( Memory::OutOfReach::Are0(entry,range) );
            assert( curr->chkBits(bits) );
        }

        Block:: ~Block() noexcept
        {
            assert(0==quantity());
        }

        void Block:: withhold() noexcept
        {
            ++nref;
        }

        bool Block:: liberate() noexcept
        {
            assert(nref>0);
            return (--nref <= 0);
        }

        size_t Block:: quantity() const noexcept
        {
            return nref;
        }

        void Block:: ldz() noexcept
        {
            Coerce(bits) = 0;
            Coerce(plan) = Plan1;
            memset(entry,0,range);
            relink();
            Coerce(curr->words) = 0;
            for(size_t i=0;i<JigAPI::Faded;++i) Coerce(dull[i]->words) = 0;
        }

        void Block:: syncDull() noexcept
        {
            for(size_t i=0;i<JigAPI::Faded;++i)
            {
                assert(0!=dull[i]);
                dull[i]->setBits(bits);
            }
        }

        void Block:: sync() noexcept
        {
            Coerce(bits) = curr->upgrade();
            syncDull();
        }


        void Block:: relink() noexcept {
            //--------------------------------------------------------------
            // set current plan
            //--------------------------------------------------------------
            Jigs &self = *this;
            Coerce( curr ) = &self[plan];

            //--------------------------------------------------------------
            // store foreign plans
            //--------------------------------------------------------------
            const Plan * const p = JigAPI::Dull[plan];
            for(size_t i=0;i<JigAPI::Faded;++i)
                dull[i] = &self[p[i]];

        }

        Block * Block:: duplicate(const Block * const block) noexcept
        {
            const size_t request = block->as<Plan8>().words*Jig8::WordBytes;
            assert(range>=request);
            memcpy(entry,block->entry,request);
            Coerce(as<Plan1>().words) = block->as<Plan1>().words;
            Coerce(as<Plan2>().words) = block->as<Plan2>().words;
            Coerce(as<Plan4>().words) = block->as<Plan4>().words;
            Coerce(as<Plan8>().words) = block->as<Plan8>().words;
            Coerce(bits) = block->bits;
            Coerce(plan) = block->plan;
            relink();
            assert(tag32()==block->tag32());
            return this;
        }


        std::ostream & operator<<(std::ostream &os, const Block &block)
        {
            block.curr->display(os);
            return os;
        }


        // compact TARGET=large, SOURCE=small
        template <typename TARGET, typename SOURCE>
        inline TARGET Compact(const SOURCE * &src) noexcept
        {
            Y_STATIC_CHECK(sizeof(TARGET)>sizeof(SOURCE),BadConfig);
            static const unsigned targetBytes   = sizeof(TARGET);
            static const unsigned sourceBytes   = sizeof(SOURCE);
            static const unsigned ratio         = targetBytes/sourceBytes;
            static const unsigned bitsPerSource = sourceBytes << 3;

            assert(0!=src);

            TARGET w = static_cast<const TARGET>( *(src++) );
            for(size_t i=1;i<ratio;++i)
                w |= static_cast<const TARGET>( *(src++) ) << (i*bitsPerSource);

            return w;
        }

        //! Scatter TARGET=small, SOURCE=large
        template <typename TARGET, typename SOURCE>
        inline void Scatter(TARGET * &tgt, SOURCE w ) noexcept
        {
            Y_STATIC_CHECK(sizeof(TARGET)<sizeof(SOURCE),BadConfig);
            static const unsigned targetBytes   = sizeof(TARGET);
            static const unsigned sourceBytes   = sizeof(SOURCE);
            static const unsigned ratio         = targetBytes/sourceBytes;
            static const unsigned bitsPerTarget = targetBytes << 3;
            static const SOURCE   mask = static_cast<SOURCE>( IntegerFor<TARGET>::Maximum );


            assert(0!=tgt);
            for(size_t i=ratio;i>0;--i)
            {
                *(tgt++) = static_cast<TARGET>(w & mask);
                w >>= bitsPerTarget;
            }

        }



        template <typename TARGET_JIG, typename SOURCE_JIG> inline
        void compact(TARGET_JIG       & target,
                     const SOURCE_JIG & source) noexcept
        {
            typedef typename TARGET_JIG::Word TARGET_WORD;
            typedef typename SOURCE_JIG::Word SOURCE_WORD;
            TARGET_WORD *       tgt    = target.word;
            const SOURCE_WORD * src    = source.word;
            for(size_t i=target.words;i>0;--i)
                *(tgt++) = Compact<TARGET_WORD,SOURCE_WORD>(src);
        }


        template <typename TARGET_JIG, typename SOURCE_JIG> inline
        void scatter(TARGET_JIG       & target,
                     const SOURCE_JIG & source) noexcept
        {
            typedef typename TARGET_JIG::Word TARGET_WORD;
            typedef typename SOURCE_JIG::Word SOURCE_WORD;
            TARGET_WORD *       tgt    = target.word;
            const SOURCE_WORD * src    = source.word;
            for(size_t i=source.words;i>0;--i)
            {
                Scatter( tgt, *(src++) );
            }
        }

        Block & Block:: to(const Plan target) noexcept
        {
            assert(curr->chkBits(bits));
            switch(plan) {

                    // SOURCE=Plan1
                case Plan1 :
                    switch(target) {
                        case Plan1: return *this;
                        case Plan2: compact( as<Plan2>(), as<Plan1>() ); break;
                        case Plan4: compact( as<Plan4>(), as<Plan1>() ); break;
                        case Plan8: compact( as<Plan8>(), as<Plan1>() ); break;
                    }
                    break;


                    // SOURCE=Plan2
                case Plan2 :
                    switch(target) {
                        case Plan1: scatter( as<Plan1>(), as<Plan2>() ); break;
                        case Plan2: return *this;
                        case Plan4: compact( as<Plan4>(), as<Plan2>() ); break;
                        case Plan8: compact( as<Plan8>(), as<Plan2>() ); break;
                    }
                    break;


                    // SOURCE=Plan4
                case Plan4 :
                    switch(target) {
                        case Plan1: scatter( as<Plan1>(), as<Plan4>() ); break;
                        case Plan2: scatter( as<Plan1>(), as<Plan4>() ); break;
                        case Plan4: return *this;
                        case Plan8: compact( as<Plan8>(), as<Plan4>() ); break;
                    }
                    break;

                    // SOURCE=Plan8
                case Plan8 :
                    switch(target) {
                        case Plan1: scatter( as<Plan1>(), as<Plan8>() ); break;
                        case Plan2: scatter( as<Plan1>(), as<Plan8>() ); break;
                        case Plan4: scatter( as<Plan1>(), as<Plan8>() ); break;
                        case Plan8: return *this;
                    }
                    break;


            }

            Coerce(plan) = target;
            relink();
            assert(curr->chkBits(bits));
            return *this;
        }

        
        const void * Block:: To(const Plan p, natural_t &n, size_t &w) noexcept
        {
            const size_t bytes = ByteCount::For(n);

            union {
                uint64_t u8;
                uint32_t u4[2];
                uint16_t u2[4];
                uint8_t  u1[8];
            } alias = { n };

            switch(p)
            {
                case Plan1: w = bytes;
                {
                    uint8_t *p = alias.u1;
                    Scatter(p,n);
                    n = alias.u8;
                } break;

                case Plan2: w = Y_ALIGN2(bytes) >> 1;
                {
                    uint16_t *p = alias.u2;
                    Scatter(p,n);
                    n = alias.u8;
                } break;

                case Plan4: w = Y_ALIGN4(bytes) >> 2;
                {
                    uint32_t *p = alias.u4;
                    Scatter(p,n);
                    n = alias.u8;
                } break;

                case Plan8: w = Y_ALIGN8(bytes) >> 3;
                    break;
            }
            return &n;
        }


        uint32_t Block:: tag32() const noexcept
        {
            uint32_t crc = 0;
            crc = CRC32::Run(crc,bits);
            crc = CRC32::Run(crc,plan);
            crc = CRC32::Run(crc,as<Plan1>().words);
            crc = CRC32::Run(crc,as<Plan2>().words);
            crc = CRC32::Run(crc,as<Plan4>().words);
            crc = CRC32::Run(crc,as<Plan8>().words);
            return crc;
        }

        uint32_t Block:: crc32() const noexcept
        {
            uint32_t crc = tag32();
            crc = CRC32::Run(crc,shift);
            crc = CRC32::Run(crc,entry);
            crc = CRC32::Run(crc,range );
            return crc;
        }


        void  Block:: clr(const size_t ibit) noexcept
        {
            static const uint8_t _msk[8] = { 255-1,255-2,255-4,255-8,255-16,255-32,255-64,255-128 };
            assert(ibit<bits);
            make<Plan1>().word[ibit>>3] &= _msk[ibit&7];
            sync();
        }
    }

}


