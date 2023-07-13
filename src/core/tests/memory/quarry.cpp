

#include "y/memory/quarry.hpp"
#include "y/memory/corpus.hpp"
#include "y/memory/album.hpp"
#include "y/utest/run.hpp"
#include "y/lockable.hpp"
#include "y/random/shuffle.hpp"
#include "y/object.hpp"
#include "y/data/list/cxx.hpp"

using namespace Yttrium;

namespace
{

    class QBlock : public Object
    {
    public:
        explicit QBlock(void *         entry,
                        const unsigned shift) noexcept :
        next(0),
        prev(0),
        p(entry),
        s(shift)
        {
        }

        virtual ~QBlock() noexcept
        {
        }

        QBlock *next;
        QBlock *prev;
        void *         p;
        const unsigned s;


    private:
        Y_DISABLE_COPY_AND_ASSIGN(QBlock);
    };

}

Y_UTEST(memory_quarry)
{
    Random::Rand ran;
    Y_SIZEOF(Memory::Quarry::Stone);
    Y_SIZEOF(Memory::Quarry::Vein);

    Memory::Album  album;
    Memory::Corpus corpus(album);
    Memory::Quarry quarry(corpus);

    std::cerr << "Required: " << Memory::Quarry::Required << std::endl;

    CxxListOf<QBlock> qblocks;

    for(size_t loop=2+ran.leq(200);loop>0;--loop)
    {
        unsigned shift = unsigned(ran.leq(16));
        void    *block = quarry.acquire(shift);
        qblocks.pushTail( new QBlock(block,shift) );
    }

    quarry.displayInfo(0);


    std::cerr << "Got #" << qblocks.size << " blocks" << std::endl;
    Random::Shuffle::List(qblocks,ran);
    while(qblocks.size)
    {
        QBlock *b = qblocks.popTail();
        quarry.release(b->p, b->s);
        delete b;
    }
    quarry.displayInfo(0);


}
Y_UDONE()
