#include "y/mpi++/api.hpp"



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

            template <typename T> inline
            void decl(const MPI_Datatype t)
            {
                const DataTypePtr ptr( new MPI::DataType( Type2Type<T>(), t) );
                (void) insert(ptr);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DataTypeDB);
        };

        static void *DataTypeWksp[ Y_WORDS_FOR(DataTypeDB) ] = { 0 };
       
        static inline DataTypeDB * GetDataTypeDB() noexcept
        {
            return static_cast<DataTypeDB *>( Memory::OutOfReach::Addr(DataTypeWksp) );
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
            throw Yttrium::Exception("[%s] not initialized with MPI::Init(...)", MPI::CallSign);

        if( MPI_SUCCESS != MPI_Init_thread(mpi_init_argc,mpi_init_argv,mpi_threading, &Coerce(threadSupport) ) )
            throw Yttrium::Exception("[%s] couldn't initialize", MPI::CallSign);
    }


}

#include "y/type/temporary.hpp"
#include "y/system/wtime.hpp"
#include "y/mkl/complex.hpp"

namespace Yttrium
{

    const char * const MPI::CallSign = "MPI";

    static inline void destructDB()
    {
        Memory::OutOfReach::Naught( GetDataTypeDB() );

    }

#if 0
    CHAR
    SIGNED_CHAR
    UNSIGNED_CHAR
    SHORT
    UNSIGNED_SHORT
    INT
    UNSIGNED
    LONG
    UNSIGNED_LONG
    LONG_LONG_INT
    LONG_LONG
    UNSIGNED_LONG_LONG
    FLOAT
    DOUBLE
    LONG_DOUBLE
    INT8_T
    INT16_T
    INT32_T
    INT64_T
    UINT8_T
    UINT16_T
    UINT32_T
    UINT64_T
    C_FLOAT_COMPLEX
    C_DOUBLE_COMPLEX
    C_LONG_DOUBLE_COMPLEX
#endif

#define Y_MPI_TDB(TYPE,DATA_TYPE) tdb->decl<TYPE>(MPI_##DATA_TYPE)
    static inline void createDB()
    {
        DataTypeDB * tdb = new ( GetDataTypeDB() ) DataTypeDB();

        try {
            Y_MPI_TDB(char,CHAR);
            Y_MPI_TDB(signed char,SIGNED_CHAR);
            Y_MPI_TDB(unsigned char,UNSIGNED_CHAR);
            Y_MPI_TDB(short,SHORT);
            Y_MPI_TDB(unsigned short,UNSIGNED_SHORT);
            Y_MPI_TDB(int,INT);
            Y_MPI_TDB(unsigned,UNSIGNED);
            Y_MPI_TDB(long,LONG);
            Y_MPI_TDB(unsigned long,UNSIGNED_LONG);
            Y_MPI_TDB(long long int,LONG_LONG_INT);
            Y_MPI_TDB(long long,LONG_LONG);
            Y_MPI_TDB(unsigned long long,UNSIGNED_LONG_LONG);
            Y_MPI_TDB(float,FLOAT);
            Y_MPI_TDB(double,DOUBLE);
            Y_MPI_TDB(long double,LONG_DOUBLE);
            Y_MPI_TDB(int8_t,INT8_T);
            Y_MPI_TDB(int16_t,INT16_T);
            Y_MPI_TDB(int32_t,INT32_T);
            Y_MPI_TDB(int64_t,INT64_T);
            Y_MPI_TDB(uint8_t,UINT8_T);
            Y_MPI_TDB(uint16_t,UINT16_T);
            Y_MPI_TDB(uint32_t,UINT32_T);
            Y_MPI_TDB(uint64_t,UINT64_T);
            Y_MPI_TDB(Complex<float>,C_FLOAT_COMPLEX);
            Y_MPI_TDB(Complex<double>,C_DOUBLE_COMPLEX);
            Y_MPI_TDB(Complex<long double>,C_LONG_DOUBLE_COMPLEX);
        }
        catch(...)
        {
            destructDB();
            throw;
        }
    }



    MPI:: ~MPI() noexcept
    {
        destructDB();
    }




    MPI &MPI:: Init(int *argc, char ***argv, const int thread_support)
    {
        if( Exists() ) throw Yttrium::Exception("[%s] already initialized",CallSign);
        const Temporary<int    *> tempArgs(mpi_init_argc,argc);
        const Temporary<char ***> tempArgv(mpi_init_argv,argv);
        const Temporary<int>      tempPara(mpi_threading,thread_support);
        return MPI::Instance();
    }

    MPI:: MPI() :
    Singleton<MPI>(),
    MPIXX(),
    Concurrent::Context( Comm_size(), Comm_rank() ),
    sizeIO(),
    getTicks( MPI_THREAD_SINGLE == threadSupport ? WallTime::Ticks : WallTime::LockedTicks ),
    traffic(),
    processorName( processor_name() ),
    parallel( size>1 ),
    primary( 0 == rank ),
    replica( 0 <  rank )
    {
        createDB();
    }

    const MPI::DataType & MPI:: get(const RTTI &rtti) const
    {
        static DataTypeDB        &tdb = *GetDataTypeDB();
        const LittleEndianAddress key( rtti );

        const DataTypePtr *ppD = tdb.search(key);
        if(0==ppD) throw MPI::Exception(MPI_ERR_TYPE, "No DataType for <%s>", rtti.name().c_str());

        return **ppD;
    }



}

