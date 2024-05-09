#include "y/ink/pixmap.hpp"
#include "y/utest/run.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/ink/image/codecs.hpp"


namespace Yttrium
{
    namespace Ink
    {

        template <size_t DeltaX, size_t DeltaY>
        class Block
        {
        public:
            static const unit_t X = DeltaX;
            static const unit_t Y = DeltaY;
            static const unit_t W = X*2+1;
            static const unit_t H = Y*2+1;
            static const size_t N = W*H;
            
            explicit Block() {}
            virtual ~Block() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Block);
        };

    }
}

using namespace Yttrium;
using namespace Ink;

Y_UTEST(block)
{
    Concurrent::Topology   topo;
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
    Slabs                  par( crew );

    Codec &IMG = Codecs::Std();

    if(argc>1)
    {
        Pixmap<RGBA> img = IMG.load(argv[1],0);
        Block<1,1>   blk;
    }

}
Y_UDONE()
