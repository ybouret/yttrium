
//! \file

#ifndef Y_Apex_Ortho_Included
#define Y_Apex_Ortho_Included 1

#include "y/apex/rational.hpp"
#include "y/container/cxx-array.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace Yttrium
{
    namespace Apex
    {

        //______________________________________________________________________
        //
        //
        //
        //! Handling orthogonal, univocal vectors
        //
        //
        //______________________________________________________________________
        struct Ortho
        {
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            typedef CxxArray<const apz,Memory::Pooled > VectorType; //!< base type for a vector
            typedef CxxArray<apq,      Memory::Dyadic>  QArrayType; //!< workspace


            //__________________________________________________________________
            //
            //
            //! univocal vector of rationals + squared norm
            //
            //__________________________________________________________________
            class Vector : public Object, public VectorType
            {
            public:
                static const char * const DerivedCallSign;
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Vector(const size_t  dims);  //!< nul vector
                explicit Vector(const Vector &other); //!< copy
                virtual ~Vector() noexcept;           //!< cleanup

                //! setup from compatible array
                template <typename ARRAY>
                inline Vector(ARRAY &arr, const AsCopy_ &):
                Object(), VectorType(arr.size()), norm2(0), next(0), prev(0)
                {
                    assert(arr.size()>0);
                    QArrayType wksp(arr,AsCopy);
                    update(wksp);
                }

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                Y_OSTREAM_PROTO(Vector); //!< display with norm2
                virtual const char *callSign() const noexcept;


                
                bool computeOrtho(Writable<apq> &v) const;



                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const apn norm2; //!< |this|^2
                Vector   *next;  //!< for list
                Vector   *prev;  //!< for list

            private:
                Y_DISABLE_ASSIGN(Vector);
                void update(QArrayType &Q);
                void clear() noexcept;
            };
        };
    }

}

#endif
