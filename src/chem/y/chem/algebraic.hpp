//! \file

#ifndef Y_Chemical_Algebra_Included
#define Y_Chemical_Algebra_Included 1

#include "y/container/matrix.hpp"
#include "y/container/cxx/array.hpp"
#include "y/apex/integer.hpp"
#include "y/data/list/cxx.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{

    namespace Chemical
    {

        struct Algebraic
        {
            typedef CxxArray<int> Coefficients;

            class Weight : public Object, public Coefficients
            {
            public:
                typedef CxxListOf<Weight> List;

                explicit Weight(const Readable<const apz> &W,
                                const Matrix<int>         &Nu);
                virtual ~Weight() noexcept;

                const Coefficients stoi;
                Weight *           next;
                Weight *           prev;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Weight);
            };



            static void Compute(Matrix<unsigned> &Q, const Matrix<int> &Nu, XMLog &xml);
            static void Compute(Weight::List     &W, const Matrix<int> &Nu, XMLog &xml);
        };

    }

}

#endif

