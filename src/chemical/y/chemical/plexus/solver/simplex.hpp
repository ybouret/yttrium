
#ifndef Y_Chemical_Simplex_Included
#define Y_Chemical_Simplex_Included 1

#include "y/chemical/plexus/solver/vertex.hpp"
#include "y/data/list/ordered.hpp"
#include "y/chemical/species.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef OrderedList<Vertex,Vertex::Comparator,OrderedListQueryHead> Vertices;

        class Simplex : public Vertices
        {
        public:
            explicit Simplex(size_t       maxEqs,
                             const size_t maxSpecies) :
            Vertices(),
            pool(),
            dims(maxSpecies)
            {
                ++maxEqs;
                while(maxEqs-- > 0) pool.store( new Vertex(dims) );
            }

            void free( Vertex * const vtx) noexcept
            {
                assert(0!=vtx);
                pool.store( vtx )->clear();
            }

            void free() noexcept {
                while(size>0) free( popTail() );
            }

            const Vertex & load(const xreal_t    cost,
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


            Vertex::Pool pool;
            const size_t dims;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Simplex);
        };

    }

}

#endif

