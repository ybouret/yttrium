
#ifndef Y_Chemical_Simplex_Included
#define Y_Chemical_Simplex_Included 1

#include "y/chemical/plexus/solver/vertex.hpp"
#include "y/data/list/ordered.hpp"
#include "y/chemical/species.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //! ordered list of vertices
        typedef OrderedList<Vertex,Vertex::Comparator,OrderedListQueryHead> Vertices;

        //______________________________________________________________________
        //
        //
        //
        //! Simplex of vertices
        //
        //
        //______________________________________________________________________
        class Simplex : public Vertices
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup for max eqs and max species
            explicit Simplex(size_t       maxEqs,
                             const size_t maxSpecies);

            //! cleanup
            virtual ~Simplex() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void free(Vertex * const) noexcept; //!< store and clean vertex
            void free()               noexcept; //!< free all vertices

            //! load a new vertex with cost and given concentration
            const Vertex & load(const xreal_t    cost,
                                const SList     &spec,
                                const XReadable &C,
                                const Level      L);

            Simplex & pop() noexcept
            {
                assert(size>0);
                free( popHead() );
                return *this;
            }
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Vertex::Pool pool; //!< pool of clean vertices
            const size_t dims; //!< common dimensions=initial speceis
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Simplex);
        };

    }

}

#endif

