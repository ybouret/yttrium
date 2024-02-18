
#include "y/mpi++/cxx.hpp"
#include <cstring>

namespace Yttrium
{
#define Y_MPIXX_SUPPORT(MODEL) case MODEL : return #MODEL

    const char * MPIXX:: ThreadSupport(const int t) noexcept
    {
        switch(t)
        {
                Y_MPIXX_SUPPORT(MPI_THREAD_SINGLE);
                Y_MPIXX_SUPPORT(MPI_THREAD_FUNNELED);
                Y_MPIXX_SUPPORT(MPI_THREAD_SERIALIZED);
                Y_MPIXX_SUPPORT(MPI_THREAD_MULTIPLE);
                
            default:
                return Core::Unknown;
        }
    }

    MPIXX:: ~MPIXX() noexcept
    {
        MPI_Finalize();
    }

    const char *MPIXX:: threadSupportText() const noexcept
    {
        return ThreadSupport(threadSupport);
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
