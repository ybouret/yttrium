#include "y/mpi++/api.hpp"


namespace Yttrium
{


    static int    *mpi_init_argc = 0;
    static char ***mpi_init_argv = 0;

    MPIXX:: MPIXX()
    {
        if(0==mpi_init_argc||0==mpi_init_argv)
            throw Yttrium::Exception("[%s] not initialized", MPI::CallSign);

        if( MPI_SUCCESS != MPI_Init(mpi_init_argc,mpi_init_argv) )
            throw Yttrium::Exception("[%s] couldn't initialize", MPI::CallSign);
    }



}

#include "y/type/temporary.hpp"

namespace Yttrium
{

    const char * const MPI::CallSign = "MPI";

    MPI:: ~MPI() noexcept {}

    MPI:: MPI() :  
    Singleton<MPI>(),
    MPIXX(),
    Concurrent::Context( Comm_size(), Comm_rank() ),
    processorName( processor_name() )
    {

    }


    MPI &MPI:: Init(int *argc, char ***argv)
    {
        if( Exists() ) throw Yttrium::Exception("[%s] already initialized",CallSign);
        const Temporary<int    *> tempArgs(mpi_init_argc,argc);
        const Temporary<char ***> tempArgv(mpi_init_argv,argv);
        return MPI::Instance();
    }

}

