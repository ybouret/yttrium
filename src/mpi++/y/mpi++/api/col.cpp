
#include "y/mpi++/api.hpp"

namespace Yttrium
{

    void MPI:: broadcast(void * const    entry,
                         const size_t    count,
                         const DataType &datatype,
                         const size_t    root)
    {
        assert( Good(entry,count) );
        assert( count <= MaxCount );

        const uint64_t mark = getTicks();
        Y_MPI_CALL(MPI_Bcast(entry, static_cast<int>(count), datatype.type, int(root), MPI_COMM_WORLD));
        const uint64_t ellapsed = getTicks() - mark;
        if(rank==root)
        {
            traffic.send.record(count*datatype.size,ellapsed);
        }
        else
        {
            traffic.recv.record(count*datatype.size,ellapsed);
        }
    }

}
