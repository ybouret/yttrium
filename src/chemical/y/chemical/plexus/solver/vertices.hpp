//! \file

#ifndef Y_Chemical_Solver_Vertices_Included
#define Y_Chemical_Solver_Vertices_Included 1


#include "y/chemical/types.hpp"
#include "y/data/list/cxx.hpp"
#include "y/data/pool/cxx.hpp"
#include "y/object.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! temporary solution
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
            typedef CxxListOf<Vertex> List; //!< alias
            typedef CxxPoolOf<Vertex> Pool; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Vertex(const size_t m); //!< setup with dimensions
            virtual ~Vertex() noexcept;      //!< cleanup


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void ldz() noexcept; //!< reset

            //! compare by increasing cost
            static SignType Compare(const Vertex * const lhs,
                                    const Vertex * const rhs) noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            xreal_t     cost; //!< whatever cost
            Vertex     *next; //!< for list/pool
            Vertex     *prev; //!< for list
            const char *info; //!< info for output

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Vertex);
        };

        //______________________________________________________________________
        //
        //
        //
        //! Temporary solutions
        //
        //
        //______________________________________________________________________
        class Vertices : public Vertex::List
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Vertices(const size_t nspc) noexcept; //!< initialize
            virtual ~Vertices() noexcept;                  //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methors
            //
            //__________________________________________________________________
            virtual void free() noexcept; //!< all back to pool
            Vertex &     push(const XReadable &cc, const xreal_t ff); //!< push new vertex

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t dims; //!< common dimensions
            Vertex::Pool pool; //!< memory cache

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Vertices);
        };


    }

}

#endif

