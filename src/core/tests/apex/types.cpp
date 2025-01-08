#include "y/apex/types.hpp"
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
#include <cstring>
#include <cerrno>

namespace Yttrium
{
    namespace Apex
    {

        enum MemoryMode
        {
            StaticMemory,
            ArchonMemory
        };

        typedef uint64_t  natural_t;
        typedef int64_t   integer_t;


        template <typename T>
        struct NumberOf
        {
            static inline size_t ToHold(const size_t bytes) noexcept
            {
                return Y_ALIGN_TO(T,bytes)/sizeof(T);
            }
        };

        struct API
        {

            static const size_t   StaticBytes = 2 * sizeof(natural_t);
            static const unsigned StaticShift = iLog2<StaticBytes>:: Value;
            static const size_t   StaticWords = StaticBytes/sizeof(void*);
        };

        enum Plan
        {
            Plan1=0,
            Plan2=1,
            Plan4=2,
            Plan8=3
        };

        template <Plan PLAN>
        class View
        {
        public:
            static const unsigned                         WordShift = PLAN;
            static const unsigned                         WordBytes = 1 << PLAN;
            typedef typename UnsignedInt<WordBytes>::Type Word;

            inline  View(void * const addr, const size_t capa) noexcept :
            word( static_cast<Word *>(addr ) ),
            count(0),
            space( capa >> WordShift ),
            priv(0)
            {
            }

            inline ~View() noexcept {}

            Word * const word;
            size_t       count;
            const size_t space;
            void        *priv;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(View);
        };

        typedef View<Plan1> View1;
        typedef View<Plan2> View2;
        typedef View<Plan4> View4;
        typedef View<Plan8> View8;

        class Views {
        public:
            Views(void * const addr, const size_t capa) noexcept:
            _1(addr,capa),
            _2(addr,capa),
            _4(addr,capa),
            _8(addr,capa)
            {
            }

            ~Views() noexcept {}

            View1 _1;
            View2 _2;
            View4 _4;
            View8 _8;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Views);
        };


        struct Bulk
        {
            void *   entry;
            unsigned shift;
        };

#define Y_Apex_Block_Bulk ( *(Bulk  *) &wksp[0] )
#define Y_Apex_Block_View ( *(Views *) &wksp[API::StaticWords] )

#define Y_Apex_Block()     \
wksp(),                    \
bulk( Y_Apex_Block_Bulk ), \
view( Y_Apex_Block_View ), \
plan( Plan1 ),             \
capa(API::StaticBytes),    \
size(0),                   \
bits(0),                   \
mode(StaticMemory),        \
addr( (void*) wksp ),      \
next(0), \
prev(0)


        class Block : public Quantized
        {
        public:
            typedef CxxListOf<Block> List;
            typedef CxxPoolOf<Block> Pool;

            explicit Block() noexcept :
            Y_Apex_Block()
            {
                zclear();
                onInit();
            }

            explicit Block(size_t requested) :
            Y_Apex_Block()
            {
                zclear();
                if(requested>capa)
                {
                    if(requested>Base2<size_t>::MaxPowerOfTwo)
                        throw Libc::Exception(ENOMEM,"requested too much");

                    requested    = NextPowerOfTwo(requested);
                    bulk.shift   = Base2<size_t>::Log(requested);
                    bulk.entry   = Memory::Archon::Acquire(bulk.shift);
                    Coerce(mode) = ArchonMemory;
                    Coerce(capa) = size_t(1) << bulk.shift;
                    Coerce(addr) = bulk.entry;
                }
                onInit();
            }




            virtual ~Block() noexcept
            {
                switch(mode)
                {
                    case StaticMemory: break;
                    case ArchonMemory:
                        memset(bulk.entry,0,capa);
                        Memory::Archon::Release(bulk.entry,bulk.shift);
                        Coerce(capa) = API::StaticBytes;
                        Coerce(mode) = StaticMemory;
                        break;
                }
                onQuit();
                zclear();
                size = 0;
                bits = 0;
            }

            void zero() noexcept
            {
                size = 0;
                bits = 0;
                Coerce(plan) = Plan1;
                memset(addr,0,capa);
            }


        private:
            void *           wksp[API::StaticWords+Y_WORDS_FOR(Views)];
            Bulk &           bulk;
        public:
            Views &          view;
            const Plan       plan;
            const size_t     capa; //!< in bytes, a power of two
            size_t           size; //!< in bytes <= capa
            size_t           bits; //!< [0..bytes*8]
            const MemoryMode mode;
        private:
            void * const     addr;
        public:
            Block * next;
            Block * prev;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Block);
            void zclear() noexcept {
                memset(wksp,0,sizeof(wksp));
            }


            void onInit() noexcept
            {
                new ( &view ) Views(addr,capa);
            }

            void onQuit() noexcept
            {
                Destruct( &view );
            }

        };


        class Blocks : public Proxy<const  Block::Pool>
        {
        public:
            static const size_t One = 1;

            explicit Blocks(const unsigned _shift) noexcept :
            shift(_shift),
            bytes(One<<shift),
            my()
            {
                assert(shift>=API::StaticShift);
                assert(shift<=Base2<size_t>::MaxShift);
            }

            virtual ~Blocks() noexcept {}

            void store(Block * const block) noexcept
            {
                assert(0!=block);
                assert(block->capa == bytes);
                my.store(block)->zero();
            }

            Block *query()
            {
                return (my.size>0) ?  my.query() : new Block(bytes);
            }



            const unsigned shift;
            const size_t   bytes;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blocks);
            Y_PROXY_DECL();
            Block::Pool my;
        };

        Y_PROXY_IMPL(Blocks,my);


        class Factory
        {
        public:
            static const unsigned MaxShift  = Base2<size_t>::MaxShift;
            static const unsigned MinShift  = API::StaticShift;
            static const unsigned NumBlocks = MaxShift-MinShift+1;
            static const unsigned Requested = NumBlocks * sizeof(Blocks);
            explicit Factory() noexcept : blocks(0), wksp()
            {
                std::cerr << "sizeof(wksp)=" << sizeof(wksp) << std::endl;
                memset(wksp,0,sizeof(wksp));
                Coerce(blocks) = static_cast<Blocks *>(Memory::OutOfReach::Addr( &wksp[0] ))-MinShift;
                for(unsigned i=MinShift;i<=MaxShift;++i)
                {
                    new (&blocks[i]) Blocks(i);
                }
            }

            Block * queryShift(unsigned shift)
            {
                if(shift>=MaxShift) throw Specific::Exception("here", "shift overflow");
                if(shift<MinShift)  shift = MinShift;
                return blocks[shift].query();
            }

            virtual ~Factory() noexcept
            {
                for(unsigned i=MaxShift;i>=MinShift;--i)
                    Destruct( &blocks[i] );
                Coerce(blocks) = 0;
                memset(wksp,0,sizeof(wksp));
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Factory);
            Blocks * const blocks;
            void   *       wksp[Y_WORDS_GEQ(Requested)];
        };

    }
}

using namespace Yttrium;
using namespace Apex;

Y_UTEST(apex_types)
{

    Y_USHOW(Apex::NumberOf<uint8_t> ::ToHold(21));
    Y_USHOW(Apex::NumberOf<uint16_t>::ToHold(21));
    Y_USHOW(Apex::NumberOf<uint32_t>::ToHold(21));
    Y_USHOW(Apex::NumberOf<uint64_t>::ToHold(21));

    Y_USHOW(Apex::API::StaticBytes);
    Y_USHOW(Apex::API::StaticShift);
    Y_USHOW(Apex::API::StaticWords);

    Y_USHOW(Memory::Archon::MinBytes);
    Y_USHOW(Memory::Archon::MinShift);

    Y_SIZEOF(Apex::View1);
    Y_SIZEOF(Apex::View2);
    Y_SIZEOF(Apex::View4);
    Y_SIZEOF(Apex::View8);

    Y_SIZEOF(Apex::Block);
    Y_SIZEOF(Apex::Blocks);

    Y_USHOW(Apex::Factory::NumBlocks);

    Apex:: Block b0;
    Apex:: Block b1(100);

    Apex:: Factory f;

    Block::List blist;
    blist.pushTail( f.queryShift(0) );


}
Y_UDONE()


