#include "y/mpi++/api.hpp"
#include "y/text/human-readable.hpp"

namespace Yttrium
{
    MPI::Monitor:: Monitor() noexcept : bytes(0), ticks(0)
    {
    }

    MPI::Monitor:: ~Monitor() noexcept
    {
        reset();
    }

    void MPI::Monitor:: reset() noexcept
    {
        Coerce(bytes)=0;
        Coerce(ticks)=0;
    }

    void MPI:: Monitor:: record(const uint64_t n, const uint64_t t) noexcept
    {
        Coerce(bytes) += n;
        Coerce(ticks) += t;
    }

    std::ostream & operator<<(std::ostream &os, const MPI::Monitor &self)
    {
        os << HumanReadable(self.bytes) << "b";
        return os;
    }

}
