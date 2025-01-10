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

        typedef uint64_t  natural_t;
        typedef int64_t   integer_t;

        enum Plan
        {
            Plan1=0,
            Plan2=1,
            Plan4=2,
            Plan8=3
        };

        class JigAPI
        {
        public:
            static const size_t Plans = 4;

        protected:
            explicit JigAPI(const size_t _count) noexcept :
            words(0), count(_count)
            {
            }

            virtual ~JigAPI() noexcept {}

        public:
            const size_t words;
            const size_t count;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(JigAPI);
        };

        template <Plan PLAN>
        class Jig : public JigAPI
        {
        public:
            static const unsigned WordShift = PLAN;
            static const unsigned WordBytes = 1 << WordShift;
            typedef typename UnsignedInt<WordBytes>::Type Word;

            explicit Jig(void * const entry, const size_t range) noexcept :
            JigAPI(range >> WordShift),
            word( static_cast<Word *>(entry) )
            {
            }

            virtual ~Jig() noexcept
            {
            }


            Word * const word;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Jig);
        };

        typedef Jig<Plan1> Jig1;
        typedef Jig<Plan2> Jig2;
        typedef Jig<Plan4> Jig4;
        typedef Jig<Plan8> Jig8;




        class Block : public Quantized
        {
        public:
            typedef CxxListOf<Block> List;
            typedef CxxPoolOf<Block> Pool;

            static const size_t One = 1;

            explicit Block(unsigned _shift) :
            shift(_shift),
            entry( Memory::Archon::Acquire( Coerce(shift) ) ),
            bits(0),
            jig1(entry, One << shift ),
            bytes( jig1.words ),
            range( jig1.count ),
            jig2(entry,range),
            jig4(entry,range),
            jig8(entry,range),
            jig(),
            next(0),
            prev(0)
            {
                jig[Plan1] = &jig1;
                jig[Plan2] = &jig2;
                jig[Plan4] = &jig4;
                jig[Plan8] = &jig8;
                
                assert( Memory::OutOfReach::Are0(entry,range) );
            }

            virtual ~Block() noexcept
            {
                ldz();
                Memory::Archon::Release(entry,shift);
                Coerce(shift) = 0;
                Coerce(entry) = 0;
            }

            void ldz() noexcept
            {
                Coerce(bits) = 0;
                //memset(entry,0,range);
            }

            const unsigned shift; //!< log2(range)
            void * const   entry; //!< memory
            size_t         bits;  //!< current number of bits
            Jig1           jig1;  //!<
            const size_t &bytes;
            const size_t &range;
            Jig2           jig2;  //!<
            Jig4           jig4;  //!<
            Jig8           jig8;  //!<
            JigAPI *       jig[JigAPI::Plans];
            Block *        next;  //!< for list/pool
            Block *        prev;  //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Block);
        };

        class Blocks : public Proxy<const Block::Pool>
        {
        public:
            explicit Blocks(const unsigned _shift) noexcept :
            Proxy<const Block::Pool>(),
            my(),
            shift(_shift)
            {
                assert(shift>=Memory::Archon::MinShift);
                assert(shift<=Base2<size_t>::MaxShift);
            }

            virtual ~Blocks() noexcept {}

            Block * query() {
                Block * const block = (my.size>0) ? my.query() : new Block(shift);
                assert( shift == block->shift );
                //assert( Memory::OutOfReach::Are0(block->entry, block->range) );
                return block;
            }

            void store(Block * const block) noexcept
            {
                assert(0!=block);
                assert(block->shift==shift);
                my.store(block)->ldz();
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blocks);
            Y_PROXY_DECL();
            Block::Pool my;
        public:
            const unsigned shift;

        };

        Y_PROXY_IMPL(Blocks,my)



    }
}

using namespace Yttrium;
using namespace Apex;

Y_UTEST(apex_types)
{

    Y_SIZEOF(Apex::Block);
    Y_SIZEOF(Apex::Blocks);

    Apex::Block block(0);


}
Y_UDONE()


