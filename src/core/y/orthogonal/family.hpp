//! \file

#ifndef Y_Orthogonal_Family_Included
#define Y_Orthogonal_Family_Included 1

#include "y/orthogonal/vector.hpp"
#include "y/data/list/cxx.hpp"

namespace Yttrium
{

    namespace Orthogonal
    {

        //______________________________________________________________________
        //
        //
        // Definition
        //
        //______________________________________________________________________
        typedef CxxListOf<Vector> Vectors; //!< base type for Family
        Y_SHALLOW_DECL(Basis);             //!< helper for constructor

        //______________________________________________________________________
        //
        //
        //
        //! Family of orthogonal vectors, size<=dimensions
        //
        //
        //______________________________________________________________________
        class Family : public Metrics, public Vectors
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Family(const size_t dims) noexcept; //!< setup with dims>0
            virtual ~Family()                  noexcept; //!< cleanup
            Family(const Family &);                      //!< copy
            Y_OSTREAM_PROTO(Family);                     //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! analyze array against existing vectors
            /**
             - load array into 'remaining'
             - compute orthogonal components from all vectors
             - return true if something is left to built
             */
            //__________________________________________________________________
            template <typename ARRAY> inline
            bool wouldAccept(ARRAY &arr)
            {
                assert(dimensions==arr.size());
                if(size>=dimensions)
                {
                    // already a base
                    return false;
                }
                else
                {
                    // will check
                    for(size_t i=dimensions;i>0;--i)
                        remaining[i] = arr[i];
                    return wouldAccept();
                }
            }


            //! check if contains vectors
            /**
             true if would accept NO vector
             */
            bool contains(const Vectors &vecs);


            //! expand family from remaining vector
            const Vector &expand();

            //! expand family from another remaining
            const Vector &expandFrom(QArrayType &);


            //______________________________________________________________
            //
            //
            // Members
            //
            //______________________________________________________________
            QArrayType   remaining; //!< workspace

        private:
            Y_DISABLE_ASSIGN(Family);
            bool wouldAccept();


        public:
            //______________________________________________________________
            //
            //! helper to extract an orthogonal basis from a WOVEn survey
            /**
             \param nvec   number of vector in the basis
             \param survey list of available vectors
             */
            //______________________________________________________________
            template <typename ARRAY_LIST> inline
            explicit Family(const Basis_     &,
                            const size_t      nvec,
                            const ARRAY_LIST &survey) :
            Metrics( survey.dimensions() ), 
            Vectors(),
            remaining(dimensions)
            {
                for( const typename ARRAY_LIST::NodeType *node = survey.head;node;node=node->next)
                {
                    if(size<nvec)
                    {
                        if( wouldAccept( *node ) ) expand();
                    }
                    else
                        break;
                }

            }


        };



    }

}

#endif
