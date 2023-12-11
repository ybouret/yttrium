


#include "y/mkl/tao/par/engine2d.hpp"

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

            Engine2D::  Engine2D(const Concurrent::ThreadContext &_) noexcept :
            Concurrent::Resource2D<size_t>(_),
            strip(0) 
            {}

            Engine2D:: ~Engine2D() noexcept {}

            void Engine2D:: activate()
            {
                if(isValid())
                {
                    const Tile &tile = **this;
                    Coerce(strip) = tile.as<Strip>(); assert(0!=strip);
                }
                else
                    Coerce(strip) = 0;
            }

            const Strip & Engine2D:: operator()(const size_t indx) const noexcept
            {
                assert(0!=strip);
                assert(indx>=1);
                assert(indx<=(**this).size);
                return strip[indx];
            }

            void Engine2D:: shutdown() noexcept
            {
                Coerce(strip) = 0;
            }

        }
    }
}

