


#include "y/mkl/tao/par/motor2d.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Tao
        {

            std::ostream &operator<<(std::ostream &os, const Strip &s)
            {
                os << "@row=" << s.irow << ",col=" << s.icol << " : +" << s.ncol;
                return os;
            }

            Motor2D::  Motor2D() noexcept : strip(0) {}
            Motor2D:: ~Motor2D() noexcept {}

            void Motor2D:: activate(const Concurrent::ThreadContext &)
            {
                const Tile &tile = **this;
                Coerce(strip) = tile.as<Strip>(); assert(0!=strip);
            }

            const Strip & Motor2D:: operator()(const size_t indx) const noexcept
            {
                assert(0!=strip);
                assert(indx>=1);
                assert(indx<=(**this).size);
                return strip[indx];
            }

            void Motor2D:: shutdown() noexcept
            {
                Coerce(strip) = 0;
            }

        }
    }
}

