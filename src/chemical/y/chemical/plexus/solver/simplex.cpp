

#include "y/chemical/plexus/solver/simplex.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Simplex:: Simplex(size_t       maxEqs,
                const size_t maxSpecies) :
        Vertices(),
        pool(),
        dims(maxSpecies)
        {
            ++maxEqs;
            while(maxEqs-- > 0) pool.store( new Vertex(dims) );
        }

        Simplex:: ~Simplex() noexcept {}


        void Simplex:: free( Vertex * const vtx) noexcept
        {
            assert(0!=vtx);
            pool.store( vtx )->clear();
        }

        void Simplex:: free() noexcept {
            while(size>0) free( popTail() );
        }

        const Vertex & Simplex:: load(const xreal_t    cost,
                                      const SList     &spec,
                                      const XReadable &C,
                                      const Level      L)
        {
            AutoPtr<Vertex> ptr = pool.size ? pool.query() : new Vertex(dims);
            Vertex         &vtx = *ptr; assert(Memory::OutOfReach::Are0(&vtx[1],vtx.size()*sizeof(xreal_t)));
            vtx.cost = cost;
            for(const SNode *sn=spec.head;sn;sn=sn->next)
            {
                const size_t * const indx = (**sn).indx;
                vtx[ indx[SubLevel] ] = C[ indx[L] ];
            }
            this->store( ptr.yield() );
            return vtx;
        }
    }

}

