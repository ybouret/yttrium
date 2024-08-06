#ifndef Y_Chemical_Vertex_Included
#define Y_Chemical_Vertex_Included 1

#include "y/chemical/types.hpp"
#include "y/data/pool/cxx.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Vertex of a Simplex
        //
        //
        //______________________________________________________________________
        class Vertex : public Object, public XArray
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxPoolOf<Vertex> Pool; //!< alias

            //! comparator
            struct Comparator
            {
                //! by decreasing cost
                SignType operator()(const Vertex * const lhs, const Vertex * const rhs) const noexcept;
            };

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Vertex(const size_t species); //!< setup for speices
            virtual ~Vertex() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void clear() noexcept; //!< reset

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            xreal_t cost; //!< cost for this vertex
            Vertex *next; //!< for list
            Vertex *prev; //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Vertex);
        };
    }

}

#endif

