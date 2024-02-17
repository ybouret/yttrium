#include "y/mpi++/api.hpp"


namespace Yttrium
{
    const char * const MPI::CallSign = "MPI";
    
    MPI:: ~MPI() noexcept
    {
        MPI_Finalize();
    }

}

