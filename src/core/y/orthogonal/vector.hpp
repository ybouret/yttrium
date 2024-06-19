//! \file

#ifndef Y_Orthogonal_Vector_Included
#define Y_Orthogonal_Vector_Included 1

#include "y/kemp/rational.hpp"
#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/orthogonal/metrics.hpp"
#include "y/quantized.hpp"

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
        typedef CxxArray<const apz,Memory::Pooled> VectorType; //!< base type for a vector
        typedef CxxArray<apq,Memory::Pooled>       QArrayType; //!< workspace
        typedef Quantized                          ObjectType; //!< alias


        //______________________________________________________________________
        //
        //
        //
        //! univocal vector of rationals + squared norm
        //
        //
        //______________________________________________________________________
        class Vector : public ObjectType, public Metrics, public VectorType
        {
        public:
            static const char * const DerivedCallSign; //!< new call sign

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Vector(const size_t  dims);  //!< nul vector
            explicit Vector(const Vector &other); //!< copy
            virtual ~Vector() noexcept;           //!< cleanup
            explicit Vector(QArrayType  &wksp);   //!< build from workspace

            //! setup from compatible array
            /**
             array of apq, apz, apn, int, unsigned
             */
            template <typename ARRAY>
            inline Vector(const CopyOf_ &, ARRAY &arr):
            ObjectType(),
            Metrics(arr.size()),
            VectorType(dimensions),
            norm2(0),
            next(0),
            prev(0)
            {
                QArrayType wksp(CopyOf,arr); // convert to apq
                update(wksp);                // univocal   apz
            }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Y_OSTREAM_PROTO(Vector);                       //!< display with norm2
            virtual const char *callSign() const noexcept; //!< overridden

            //__________________________________________________________________
            //
            //! v <-  v - ( (this*v)/ |this|^2) * this
            /**
             \return true is v had orthogonal component
             */
            //__________________________________________________________________
            bool computeOrtho(Writable<apq> &v) const;

            //__________________________________________________________________
            //
            //! colinearity iff no orthogonal components
            //__________________________________________________________________
            template <typename ARRAY>
            inline bool isColinear(ARRAY &arr) const
            {
                QArrayType v(CopyOf,arr);
                return !computeOrtho(v);
            }

            //__________________________________________________________________
            //
            //! dot product
            //__________________________________________________________________
            friend apz operator* (const Vector &lhs, const Vector &rhs);

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
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



#endif
