

#include "y/apex/block.hpp"
#include "y/check/static.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Apex
    {

        Block:: Briefly:: Briefly(const Plan which, Block &block) noexcept :
        plan(block.plan),
        host(block)
        {
            block.to(which);
        }

        Block:: Briefly:: ~Briefly() noexcept
        {
            host.to(plan);
        }


        Block:: Block(const unsigned _shift) :
        DataBlock(_shift),
        Jigs(entry,range),
        bytes(as<Plan1>().words),
        next(0),
        prev(0)
        {
            relink();
            assert( Memory::OutOfReach::Are0(entry,range) );
            assert( curr->chkBits(bits) );
        }

        Block:: ~Block() noexcept
        {
        }

        void Block:: ldz() noexcept
        {
            Coerce(bits) = 0;
            memset(entry,0,range);
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
            assert(range>=block->bytes);
            switch(block->plan) {
                case Plan1: memcpy(entry,block->entry,block->as<Plan1>().words);                 break;
                case Plan2: memcpy(entry,block->entry,block->as<Plan2>().words*Jig2::WordBytes); break;
                case Plan4: memcpy(entry,block->entry,block->as<Plan4>().words*Jig4::WordBytes); break;
                case Plan8: memcpy(entry,block->entry,block->as<Plan8>().words*Jig8::WordBytes); break;
            }
            Coerce(as<Plan1>().words) = block->as<Plan1>().words;
            Coerce(as<Plan2>().words) = block->as<Plan2>().words;
            Coerce(as<Plan4>().words) = block->as<Plan4>().words;
            Coerce(as<Plan8>().words) = block->as<Plan8>().words;
            Coerce(bits) = block->bits;
            Coerce(plan) = block->plan;
            relink();
            return this;
        }


        std::ostream & operator<<(std::ostream &os, const Block &block)
        {
            block.curr->display(os);
            return os;
        }


        // compact TARGET=large, SOURCE=small
        template <typename TARGET, typename SOURCE>
        inline TARGET Compact(const SOURCE * &src ) noexcept
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
    }

}


