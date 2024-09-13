
#include "y/chemical/plexus/solver/vertices.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Vertex:: Vertex(const size_t m) :
        Quantized(),
        XArray(m),
        cost(),
        next(0),
        prev(0),
        info(0)
        {
        }


        Vertex:: ~Vertex() noexcept
        {
        }

        void  Vertex:: ldz() noexcept
        {
            ld( cost=0 );
            info = 0;
        }

        SignType  Vertex:: Compare(const Vertex * const lhs,
                                   const Vertex * const rhs) noexcept
        {
            return Comparison::Increasing(lhs->cost, rhs->cost);
        }

    }

}


namespace Yttrium
{
    namespace Chemical
    {

        Vertices:: Vertices(const size_t nspc) noexcept :
        Vertex::List(),
        dims(nspc),
        pool()
        {
        }

        Vertices:: ~Vertices() noexcept
        {
        }

        void Vertices:: free() noexcept
        {
            while(size>0) pool.store( popTail() )->ldz();
        }

        Vertex & Vertices:: push(const XReadable &cc, const xreal_t ff)
        {
            Vertex *v =  pushTail( pool.size>0 ? pool.query() : new Vertex(dims) );
            v->ld(cc);
            v->cost = ff;
            v->info = 0;
            return *v;
        }

    }

}


