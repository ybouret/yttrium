

//! \file

#ifndef Y_MKL_TAO_Motor2D_Included
#define Y_MKL_TAO_Motor2D_Included 1

#include "y/concurrent/engine/in2d.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Tao
        {
            struct Strip
            {
                size_t icol;
                size_t irow;
                size_t ncol;
                size_t cend;
                friend std::ostream &operator<<(std::ostream &os, const Strip &s);
            };

            class Motor2D : public Concurrent::Engine2D<size_t>
            {
            public:
                explicit Motor2D() noexcept;
                virtual ~Motor2D() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Motor2D);
                const Strip * const strip;
                virtual void activate(const Concurrent::ThreadContext &);

            };
        }
    }
}

#endif
