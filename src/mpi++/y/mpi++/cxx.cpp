
#include "y/mpi++/cxx.hpp"

namespace Yttrium
{

    MPIXX:: ~MPIXX() noexcept
    {
        MPI_Finalize();
    }



}

#include "y/mpi++/api.hpp"
namespace Yttrium
{
    size_t MPIXX:: Comm_size() const
    {
        int sz = 0;
        Y_MPI_CALL( MPI_Comm_size(MPI_COMM_WORLD, &sz) );
        return sz;
    }

    size_t  MPIXX:: Comm_rank() const
    {
        int rk = 0;
        Y_MPI_CALL( MPI_Comm_rank(MPI_COMM_WORLD, &rk) );
        return rk;
    }
    

    static char   mpi_processor_name[MPI_MAX_PROCESSOR_NAME] = { 0 };
 
    const char * MPIXX:: processor_name() const
    {
        memset(mpi_processor_name,0,sizeof(mpi_processor_name));
        int len = 0;
        Y_MPI_CALL( MPI_Get_processor_name(mpi_processor_name,&len) );
        return mpi_processor_name;
    }


}
