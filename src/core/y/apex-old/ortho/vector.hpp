//! \file

#ifndef Y_Apex_Ortho_Vector_Included
#define Y_Apex_Ortho_Vector_Included 1

#include "y/apex/rational.hpp"
#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/apex/ortho/metrics.hpp"

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
            typedef CxxArray<const apz> VectorType; //!< base type for a vector
            typedef CxxArray<apq>       QArrayType; //!< workspace

          

            //__________________________________________________________________
            //
            //
            //! univocal vector of rationals + squared norm
            //
            //__________________________________________________________________
            class Vector : public Object, public Metrics, public VectorType
            {
            public:
                static const char * const DerivedCallSign; //!< new call sign

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Vector(const size_t  dims);  //!< nul vector
                explicit Vector(const Vector &other); //!< copy
                virtual ~Vector() noexcept;           //!< cleanup
                explicit Vector(QArrayType  &wksp);   //!< build from workspace

                //! setup from compatible array
                template <typename ARRAY>
                inline Vector(const CopyOf_ &, ARRAY &arr):
                Object(),
                Metrics(arr.size()),
                VectorType(dimensions),
                norm2(0),
                next(0),
                prev(0)
                {
                    QArrayType wksp(CopyOf,arr);
                    update(wksp);
                }

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                Y_OSTREAM_PROTO(Vector);                       //!< display with norm2
                virtual const char *callSign() const noexcept; //!< overridden

                //______________________________________________________________
                //
                //! v <-  v - ( (this*v)/ |this|^2) * this
                /**
                 \return true is v had orthogonal component
                 */
                //______________________________________________________________
                bool computeOrtho(Writable<apq> &v) const;

                //! colinearity iff no orthogonal components
                template <typename ARRAY>
                inline bool isColinear(ARRAY &arr) const
                {
                    QArrayType v(CopyOf,arr);
                    return !computeOrtho(v);
                }

                //! dot product
                friend Integer operator* (const Vector &lhs, const Vector &rhs);

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const apn    norm2;  //!< |this|^2
                const apn    norm1;  //!< |this|_1
                const size_t numPos; //!< positive coefficients
                const size_t numNeg; //!< negative coefficients
                const size_t ncoeff; //!< number of coefficients
                Vector   *   next;   //!< for list
                Vector   *   prev;   //!< for list

            private:
                Y_DISABLE_ASSIGN(Vector);
                void update(QArrayType &Q);
                void clear() noexcept;
            };
        };
    }

}

#endif
