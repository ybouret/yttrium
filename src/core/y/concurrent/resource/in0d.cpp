
#include "y/concurrent/resource/in0d.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        namespace Nucleus
        {

            Ponctual::  Ponctual() noexcept {}
            Ponctual:: ~Ponctual() noexcept {}

            std::ostream & operator<<(std::ostream &os, const Ponctual &)
            {
                return os << "(ponctual)";
            }
        }

        Resource0D:: ~Resource0D() noexcept {}

        Resource0D:: Resource0D(const ThreadContext &ctx) : BaseType(ctx) {}

    }

}
