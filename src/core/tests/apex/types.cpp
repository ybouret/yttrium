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


        class Block : public Quantized
        {
        public:
            typedef CxxListOf<Block> List;
            typedef CxxPoolOf<Block> Pool;

            static const size_t One = 1;

            explicit Block(unsigned _shift) :
            bits(0),
            shift(_shift),
            entry( Memory::Archon::Acquire( Coerce(shift) ) ),
            range( One << shift ),
            next(0),
            prev(0)
            {
                assert( Memory::OutOfReach::Are0(entry,range) );
            }

            virtual ~Block() noexcept
            {
                ldz();
                Memory::Archon::Release(entry,shift);
                Coerce(shift) = 0;
                Coerce(entry) = 0;
                Coerce(range) = 0;
            }

            void ldz() noexcept
            {
                Coerce(bits) = 0;
                memset(entry,0,range);
            }

            const size_t   bits;
            const unsigned shift;
            void * const   entry;
            const size_t   range;
            Block *        next;
            Block *        prev;

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
                return (my.size>0) ? my.query() : new Block(shift);
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


