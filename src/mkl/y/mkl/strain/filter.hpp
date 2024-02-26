//! \file

#ifndef Y_MKL_Strain_Filter_Included
#define Y_MKL_Strain_Filter_Included 1

#include "y/container/readable.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Strain
        {
            

            template <typename T>
            class Filter
            {
            public:
                explicit Filter();
                virtual ~Filter() noexcept;




            private:
                class Code;
                Code *code;
                Y_DISABLE_COPY_AND_ASSIGN(Filter);
            };
        }
    }
}

#endif

