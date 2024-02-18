#include "y/mpi++/api.hpp"


namespace Yttrium
{
    void MPI::Send(const void *const data, const size_t count, MPI_Datatype datatype, const int destination, const int tag)
    {
        assert( Good(data,count) );
        assert(count<=MaximumSize);

        MPI_Send(data, static_cast<int>(count), datatype, destination, tag, MPI_COMM_WORLD);
    }
}



