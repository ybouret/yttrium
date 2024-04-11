
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


        class Slab : public Concurrent::Frame2D<unit_t>
        {
        public:
            explicit Slab(const ThreadContext &ctx) noexcept : Concurrent::Frame2D<unit_t> (ctx)
            {
            }

            virtual ~Slab() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Slab);
        };

        class Slabs : public Concurrent::SIMT<Slab>
        {
        public:
            explicit Slabs(const Concurrent::SharedLoop &csl) : Concurrent::SIMT<Slab>(csl) {}
            virtual ~Slabs() noexcept {}

            void setup(const Area &area)
            {
                assign(area.lower(),area.upper());
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Slabs);
        };





    }

}


Y_UTEST(tess)
{
    Concurrent::Topology   topo;
    Concurrent::SharedLoop mono = new Concurrent::Mono();
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);

    Ink::Slabs seqSlabs( mono );
    Ink::Slabs parSlabs( crew );

    Ink::Pixmap<int> ipix(5,12);
    seqSlabs.setup(ipix);
    std::cerr << seqSlabs << std::endl;
    parSlabs.setup(ipix);
    std::cerr << parSlabs << std::endl;

}
Y_UDONE()
