
//! \file

#ifndef Y_Apex_Ortho_Family_Included
#define Y_Apex_Ortho_Family_Included 1

#include "y/apex/ortho/vector.hpp"
#include "y/data/list/cxx.hpp"

namespace Yttrium
{
    
    namespace Apex
    {

        namespace Ortho
        {
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            typedef CxxListOf<Vector> Vectors; //!< base type for Family

            //__________________________________________________________________
            //
            //
            //
            //! Family of orthogonal vectors
            //
            //
            //__________________________________________________________________
            class Family : public Vectors
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Family(const size_t dims) noexcept; //!< setup with dims>0
                virtual ~Family()                  noexcept; //!< cleanup
                Family(const Family &);                      //!< copy
                Y_OSTREAM_PROTO(Family);                     //!< display

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //______________________________________________________________
                //
                //! analyze array against existing vectors
                /**
                 - load array into 'remaining'
                 - compute orthogonal components from all vectors
                 - return true if something is left
                 */
                //______________________________________________________________
                template <typename ARRAY> inline
                bool wouldAccept(ARRAY &arr)
                {
                    assert(remaining.size()==arr.size());
                    for(size_t i=dimension;i>0;--i)
                        remaining[i] = arr[i];
                    return wouldAccept();
                }

                //! expand family from remaining vector
                const Vector &expand();


                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const size_t dimension; //!< space dimension
                QArrayType   remaining; //!< workspace

            private:
                Y_DISABLE_ASSIGN(Family);
                bool wouldAccept();
            };

        }

    }

}

#endif
