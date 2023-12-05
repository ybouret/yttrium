
#include "y/concurrent/engine/interface.hpp"
#include "y/system/error.hpp"
#include <cerrno>

namespace Yttrium
{
    namespace Concurrent
    {

        namespace Nucleus
        {
            Engine:: ~Engine() noexcept {}
            Engine::  Engine() noexcept : access(0) {}

            Lockable & Engine:: sync() const noexcept
            {
                if(!access) Libc::CriticalError(EACCES, "No lockable access in Engine");
                return Coerce(*access);
            }
        }



    }

}
