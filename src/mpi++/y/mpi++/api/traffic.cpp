#include "y/mpi++/api.hpp"

namespace Yttrium
{
    /**/ MPI:: Traffic::  Traffic() noexcept : send(), recv() {}
    /**/ MPI:: Traffic:: ~Traffic() noexcept {}
    void MPI:: Traffic::  reset()   noexcept { send.reset(); recv.reset(); }

    std::ostream & operator<<(std::ostream &os, const MPI::Traffic &self)
    {
        os << "send: " << self.send << " | recv: " << self.recv;
        return os;
    }
}
