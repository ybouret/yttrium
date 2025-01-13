#include "y/apex/jig/api.hpp"

#include "y/utest/run.hpp"
#include "y/calculus/base2.hpp"
#include "y/calculus/align.hpp"
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
#include "y/check/static.hpp"
#include "y/random/park-miller.hpp"

#include <cstring>

namespace Yttrium
{
    namespace Apex
    {


  
        template <Plan PLAN>
        class Jig : public JigAPI
        {
        public:
            static const unsigned                         WordShift = PLAN;
            static const unsigned                         WordBytes = (1 << WordShift);
            static const unsigned                         WordBits  = WordBytes << 3;
            typedef typename UnsignedInt<WordBytes>::Type Word;

            inline explicit Jig(void * const entry, const size_t range) noexcept :
            JigAPI(range >> WordShift),
            word( static_cast<Word *>(entry) )
            {
                assert( IsPowerOfTwo(range) );

            }

            inline virtual ~Jig() noexcept
            {
            }

            static inline size_t BytesToWords(const size_t bytes) noexcept
            {
                return  (Y_ALIGN_TO(Word,bytes)) >> WordShift;
            }

            virtual void display(std::ostream &os)  const  {
                os << '[';
                if(words<=0)
                {
                    const Word zero = 0;
                    os << Hexadecimal(zero);
                }
                else
                {
                    size_t i = words;
                    while(i-- > 0)
                    {
                        os << Hexadecimal(word[i]);
                        if(i>0) os << ' ';
                    }
                }
                os << ']';
            }

            virtual void setBits(const size_t bits) noexcept
            {
                Coerce(words) = BytesToWords( Y_Apex_Bytes_For(bits) );
            }

            virtual size_t upgrade() noexcept
            {
                size_t &num = (Coerce(words) = count);
                size_t  msi = num-1;
                Word    msw = 0;

                while(num>0)
                {
                    msw = word[msi];
                    if(msw>0) break;
                    --num;
                    --msi;
                }

                if(num<=0)
                {
                    return 0;
                }
                else
                {
                    return msi * WordBits + BitCount::For(msw);
                }
            }

#define Y_APEX_JIG_ASSERT(EXPR) do { if( !(EXPR) ) { std::cerr << "Jig<" << PLAN << "> failure '" << #EXPR << "'" << std::endl; return false; } } while(false)
            inline bool chkBits(const size_t bits) const noexcept {

                Y_APEX_JIG_ASSERT(words<=count);

                for(size_t i=words+1;i<count;++i) {
                    Y_APEX_JIG_ASSERT(0==word[i]);
                }

                if(words>0) {
                    const size_t msi = words-1;
                    const Word   msw = word[msi];
                    Y_APEX_JIG_ASSERT(0!=msw);
                    Y_APEX_JIG_ASSERT(bits==WordBits*msi+BitCount::For(msw));
                }
                else
                {
                    Y_APEX_JIG_ASSERT(0==bits);
                }
                return true;
            }


            Word * const word;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Jig);
        };

        typedef Jig<Plan1> Jig1;
        typedef Jig<Plan2> Jig2;
        typedef Jig<Plan4> Jig4;
        typedef Jig<Plan8> Jig8;


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



        class Jigs {
        public:
            static const size_t JigSize = sizeof(Jig1);
            static const size_t Measure = JigAPI::Plans * JigSize;

            explicit Jigs(void * const entry,  const size_t range) noexcept :
            addr(0),
            wksp()
            {
                Coerce(addr) = static_cast<char *>( Memory::OutOfReach::Addr( &wksp[0] ) );
                char  *p = addr;
                new (p)            Jig1(entry,range);
                new (p += JigSize) Jig2(entry,range);
                new (p += JigSize) Jig4(entry,range);
                new (p += JigSize) Jig8(entry,range);
            }

            template <Plan PLAN> inline
            Jig<PLAN> & as() noexcept {
                return *(Jig<PLAN> *) &addr[JigSize*PLAN];
            }

            template <Plan PLAN> inline
            const Jig<PLAN> & as() const noexcept {
                return *(const Jig<PLAN> *) &addr[JigSize*PLAN];
            }


            JigAPI & operator[](const Plan plan) noexcept {
                switch(plan) {
                    case Plan1: break;
                    case Plan2: return as<Plan2>();
                    case Plan4: return as<Plan4>();
                    case Plan8: return as<Plan8>();
                }
                return as<Plan1>();
            }

            virtual ~Jigs() noexcept
            {

            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Jigs);
            char * const addr;
            void *       wksp[ Y_WORDS_GEQ(Measure) ];
        };





        class DataBlock : public Quantized
        {
        public:
            static const size_t      MinRange = 2 * sizeof(natural_t);
            static const unsigned    MinShift = iLog2<MinRange>::Value;
            static const unsigned    MaxShift = Base2<size_t>::MaxShift;
            static const size_t      MaxBytes = Base2<size_t>::MaxPowerOfTwo;

            static const size_t One = 1;
        protected:

            explicit DataBlock(const unsigned _shift) :
            shift( Max(_shift,MinShift) ),
            entry( Memory::Archon::Acquire( Coerce(shift) ) ),
            bits(0),
            plan(Plan1),
            curr(0),
            dull(),
            range( One << shift )
            {
                assert( Memory::OutOfReach::Are0(entry,range) );
            }

        public:
            virtual ~DataBlock() noexcept
            {
                Memory::Archon::Release(entry,shift);
            }

            const unsigned shift;               //!< log2(range)
            void * const   entry;               //!< memory
            const size_t   bits;                //!< current number of bits
            const Plan     plan;                //!< current plan
            JigAPI * const curr;                //!< current API
            JigAPI *       dull[JigAPI::Faded]; //!< other dull API
            const size_t   range;               //!< 2^shift

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DataBlock);
        };

        class Block : public DataBlock, public Jigs
        {
        public:

            typedef CxxListOf<Block> List;
            typedef CxxPoolOf<Block> Pool;

            static const size_t One = 1;

            explicit Block(const unsigned _shift) :
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

            virtual ~Block() noexcept
            {
            }

            Y_OSTREAM_PROTO(Block);


            //! set all to zero, keep plan for futrure use
            void ldz() noexcept
            {
                Coerce(bits) = 0;
                memset(entry,0,range);
                Coerce(curr->words) = 0;
                for(size_t i=0;i<JigAPI::Faded;++i) Coerce(dull[i]->words) = 0;
            }

            //! set curr and dull from plan
            void relink() noexcept {
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

            void syncDull() noexcept
            {
                for(size_t i=0;i<JigAPI::Faded;++i)
                {
                    assert(0!=dull[i]);
                    dull[i]->setBits(bits);
                }
            }

            //! compute bits from current plan and update dull
            void sync() noexcept
            {
                Coerce(bits) = curr->upgrade();
                syncDull();
            }

            //! change representation
            void to(const Plan target) noexcept
            {
                assert(curr->chkBits(bits));
                switch(plan) {

                        // SOURCE=Plan1
                    case Plan1 :
                        switch(target) {
                            case Plan1: return;
                            case Plan2: compact( as<Plan2>(), as<Plan1>() ); break;
                            case Plan4: compact( as<Plan4>(), as<Plan1>() ); break;
                            case Plan8: compact( as<Plan8>(), as<Plan1>() ); break;
                        }
                        break;


                        // SOURCE=Plan2
                    case Plan2 :
                        switch(target) {
                            case Plan1: scatter( as<Plan1>(), as<Plan2>() ); break;
                            case Plan2: return;
                            case Plan4: compact( as<Plan4>(), as<Plan2>() ); break;
                            case Plan8: compact( as<Plan8>(), as<Plan2>() ); break;
                        }
                        break;


                        // SOURCE=Plan4
                    case Plan4 :
                        switch(target) {
                            case Plan1: scatter( as<Plan1>(), as<Plan4>() ); break;
                            case Plan2: scatter( as<Plan1>(), as<Plan4>() ); break;
                            case Plan4: return;
                            case Plan8: compact( as<Plan8>(), as<Plan4>() ); break;
                        }
                        break;

                        // SOURCE=Plan8
                    case Plan8 :
                        switch(target) {
                            case Plan1: scatter( as<Plan1>(), as<Plan8>() ); break;
                            case Plan2: scatter( as<Plan1>(), as<Plan8>() ); break;
                            case Plan4: scatter( as<Plan1>(), as<Plan8>() ); break;
                            case Plan8: return;
                        }
                        break;


                }

                Coerce(plan) = target;
                relink();
                assert(curr->chkBits(bits));
            }

            const size_t  &bytes;
            Block *        next;                //!< for list/pool
            Block *        prev;                //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Block);
        };

        std::ostream & operator<<(std::ostream &os, const Block &block)
        {
            block.curr->display(os);
            return os;
        }


        class Blocks : public Proxy<const Block::Pool>
        {
        public:
            explicit Blocks(const unsigned _shift) noexcept :
            Proxy<const Block::Pool>(),
            my(),
            shift(_shift)
            {
                assert(shift>=Block::MinShift);
                assert(shift<=Block::MaxShift);
            }

            virtual ~Blocks() noexcept {}

            Block * query() {
                Block * const block = (my.size>0) ? my.query() : new Block(shift);
                assert( shift == block->shift );
                assert( Memory::OutOfReach::Are0(block->entry, block->range) );
                return block;
            }

            void store(Block * const block) noexcept
            {
                assert(0!=block);
                assert(block->shift==shift);
                my.store(block)->ldz();
            }

            void gc() noexcept {
                Block::List L;
                Rework::PoolToList(L,my);
                MergeSort::Call(L,ListOps::IncreasingAddresses<Block>);
                ListOps::CheckIncreasingAddresses(L);
                const size_t maxSize = 1 + (L.size>>1);
                while(L.size>maxSize) delete L.popTail();
                Rework::ListToPool(my,L);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blocks);
            Y_PROXY_DECL();
            Block::Pool my;
        public:
            const unsigned shift;

        };

        Y_PROXY_IMPL(Blocks,my)

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


