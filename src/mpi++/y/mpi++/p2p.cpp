#include "y/mpi++/api.hpp"


namespace Yttrium
{
    void MPI::Send(const void *const data,
                   const size_t      count,
                   const DataType &  datatype,
                   const size_t      destination,
                   const int         tag)
    {
        assert( Good(data,count) );
        assert(count<=MaximumSize);
        const uint64_t mark = getTicks();
        Y_MPI_CALL(MPI_Send(data, static_cast<int>(count), datatype.type, int(destination), tag, MPI_COMM_WORLD));
        traffic.send.record(count*datatype.size,getTicks()-mark);
    }

    void  MPI::Recv(void *             data,
                    const size_t       count,
                    const DataType    &datatype,
                    const size_t       source,
                    const int          tag)
    {
        assert( Good(data,count) );
        assert(count<=MaximumSize);
        MPI_Status     status;
        const uint64_t mark  = getTicks();
        Y_MPI_CALL(MPI_Recv(data, static_cast<int>(count), datatype.type, int(source), tag, MPI_COMM_WORLD, &status));
        traffic.recv.record(count*datatype.size,getTicks()-mark);
    }
}



