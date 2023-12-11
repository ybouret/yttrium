

#include "y/mkl/tao/par/engine1d.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Tao
        {
            Engine1D::  Engine1D(const Concurrent::ThreadContext &_) noexcept :
            Concurrent::Resource1D<size_t>(_),
            anonymous(0)
            {}
            
            Engine1D:: ~Engine1D() noexcept {}


            void Engine1D:: activate()
            {
                
            }

            void Engine1D:: shutdown() noexcept
            {

            }
        }
    }
}

