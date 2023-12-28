
//! \file

#ifndef Y_MKL_Filter_Smooth_Included
#define Y_MKL_Filter_Smooth_Included 1


#include "y/mkl/api.hpp"
#include "y/container/readable.hpp"

namespace Yttrium
{
    namespace MKL
    {

        template <typename T>
        class Smooth : public Readable<T>
        {
        public:
            explicit Smooth();
            virtual ~Smooth() noexcept;

            void  run(const T           &t0,
                      const Readable<T> &t,
                      const Readable<T> &z,
                      const size_t       degree);

            virtual const char * callSign()               const noexcept; //!< "Smooth"
            virtual size_t       size()                   const noexcept; //!< 3
            virtual const T &    operator[](const size_t) const noexcept; //!< [1:3]

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Smooth);
            class Code;
            Code *code;
        };
    }

}

#endif
