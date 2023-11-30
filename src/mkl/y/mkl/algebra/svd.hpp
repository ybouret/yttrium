
//! \file

#ifndef Y_MKL_SVD_Included
#define Y_MKL_SVD_Included 1

#include "y/container/matrix.hpp"

namespace Yttrium
{
    namespace MKL
    {
        //______________________________________________________________________
        //
        //
        //
        //! SVD decomposition
        //
        //
        //______________________________________________________________________
        template <typename T>
        class SVD
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit SVD();
            virtual ~SVD() noexcept;

            bool build(Matrix<T> &a, Writable<T> &w, Matrix<T> &v);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(SVD);
            class Code;
            Code *code;
        };

    }

}


#endif
