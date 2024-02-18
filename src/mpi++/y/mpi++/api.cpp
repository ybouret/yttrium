#include "y/mpi++/api.hpp"


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

}

namespace Yttrium
{
    /**/ MPI:: Traffic::  Traffic() noexcept : send(), recv() {}
    /**/ MPI:: Traffic:: ~Traffic() noexcept {}
    void MPI:: Traffic::  reset()   noexcept { send.reset(); recv.reset(); }
}

#include "y/ptr/ark.hpp"
#include "y/associative/suffix/set.hpp"

namespace Yttrium
{
    MPI:: DataType:: ~DataType() noexcept {}
    const MPI:: DataKey & MPI:: DataType::key() const noexcept { return uuid; }

    namespace
    {

        typedef ArkPtr<MPI::DataKey,MPI::DataType>  DataTypePtr;
        typedef SuffixSet<MPI::DataKey,DataTypePtr> DataTypeSet;

        class DataTypeDB : public DataTypeSet
        {
        public:
            explicit DataTypeDB() : DataTypeSet() {}
            virtual ~DataTypeDB() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DataTypeDB);
        };

        static void *DataTypeWksp[ Y_WORDS_FOR(DataTypeDB) ] = { 0 };
        static inline DataTypeDB & GetDataTypeDB() noexcept
        {
            return *static_cast<DataTypeDB *>( Memory::OutOfReach::Addr(DataTypeWksp) );
        }

    }

}

namespace Yttrium
{


    static int    *mpi_init_argc =  0;
    static char ***mpi_init_argv =  0;
    static int     mpi_threading = -1;

    MPIXX:: MPIXX() : threadSupport(-1)
    {
        if(0==mpi_init_argc || 0==mpi_init_argv)
            throw Yttrium::Exception("[%s] not initialized", MPI::CallSign);

        if( MPI_SUCCESS != MPI_Init_thread(mpi_init_argc,mpi_init_argv,mpi_threading, &Coerce(threadSupport) ) )
            throw Yttrium::Exception("[%s] couldn't initialize", MPI::CallSign);
    }


}

#include "y/type/temporary.hpp"
#include "y/system/wtime.hpp"

namespace Yttrium
{

    const char * const MPI::CallSign = "MPI";

    MPI:: ~MPI() noexcept 
    {
        Memory::OutOfReach::Naught( & GetDataTypeDB() );
    }

    MPI:: MPI() :  
    Singleton<MPI>(),
    MPIXX(),
    Concurrent::Context( Comm_size(), Comm_rank() ),
    getTicks( MPI_THREAD_SINGLE == threadSupport ? WallTime::Ticks : WallTime::LockedTicks ),
    traffic(),
    processorName( processor_name() )
    {
        new ( &GetDataTypeDB() ) DataTypeDB();
    }


    MPI &MPI:: Init(int *argc, char ***argv, const int thread_support)
    {
        if( Exists() ) throw Yttrium::Exception("[%s] already initialized",CallSign);
        const Temporary<int    *> tempArgs(mpi_init_argc,argc);
        const Temporary<char ***> tempArgv(mpi_init_argv,argv);
        const Temporary<int>      tempPara(mpi_threading,thread_support);
        return MPI::Instance();
    }

}

