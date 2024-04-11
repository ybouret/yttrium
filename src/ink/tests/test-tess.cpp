
#include "y/ink/pixmap.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/concurrent/loop/simt.hpp"
#include "y/concurrent/frame/2d.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"


using namespace Yttrium;

namespace Yttrium
{

    namespace Ink
    {

        typedef Concurrent::Frame2D<unit_t> PixFrame;

        class Slab : public PixFrame
        {
        public:
            explicit Slab(const ThreadContext &ctx) noexcept : PixFrame(ctx)
            {
            }

            virtual ~Slab() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Slab);
        };

        typedef Concurrent::SIMT<Slab> Slabs;


    }

}


Y_UTEST(tess)
{
    Concurrent::Topology topo;
    Concurrent::SharedLoop mono = new Concurrent::Mono();
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);

    Ink::Slabs seqSlabs( mono );
    Ink::Slabs parSlabs( mono );


}
Y_UDONE()
