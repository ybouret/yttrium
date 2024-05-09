#include "y/ink/pixmap.hpp"
#include "y/utest/run.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/ink/image/codecs.hpp"
#include "y/color/channels.hpp"

namespace Yttrium
{
    namespace Ink
    {

        template <size_t DeltaX, size_t DeltaY>
        class Block
        {
        public:
            static const unit_t X  = DeltaX;
            static const unit_t Y  = DeltaY;
            static const unit_t X0 = -X;
            static const unit_t Y0 = -Y;
            static const unit_t W  = X*2+1;
            static const unit_t H  = Y*2+1;
            static const size_t N  = W*H;

            explicit Block() {}
            virtual ~Block() noexcept {}


            template <typename T> inline
            void load(T *              target,
                      const Pixmap<T> &source,
                      const Coord      origin) const
            {
                assert(0!=target);
                for(unit_t y=Y;y>=Y0;--y)
                {
                    const PixRow<T> &row = source[origin.y+y];
                    for(unit_t x=X;x>=X0;--x)
                    {
                        *(target++) = row[x];
                    }
                }
            }

            

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
