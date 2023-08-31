
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

            typedef CxxListOf<Vector> Vectors;

            class Family : public Vectors
            {
            public:

                explicit Family(const size_t dims) noexcept; //!< setup with dims>0
                virtual ~Family()                  noexcept; //!< cleanup
                Family(const Family &);                      //!< copy
                Y_OSTREAM_PROTO(Family);                     //!< display

                template <typename ARRAY> inline
                bool wouldAccept(ARRAY &arr)
                {
                    assert(remaining.size()==arr.size());
                    for(size_t i=dimension;i>0;--i)
                        remaining[i] = arr[i];
                    return wouldAccept();
                }

                const Vector &expand();



                const size_t dimension;
                QArrayType   remaining;

            private:
                Y_DISABLE_ASSIGN(Family);
                bool wouldAccept();
            };

        }

    }

}

#endif
