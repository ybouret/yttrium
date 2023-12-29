
//! \file

#ifndef Y_MKL_Filter_Smooth_Arc_Included
#define Y_MKL_Filter_Smooth_Arc_Included 1


#include "y/mkl/filter/smooth.hpp"
#include "y/mkl/v2d.hpp"

namespace Yttrium
{
    namespace MKL
    {

        template <typename T>
        class SmoothArc
        {
        public:
            explicit  SmoothArc();
            virtual  ~SmoothArc() noexcept;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(SmoothArc);
            class Code;
            Code *code;
        };

    }

}

#endif

