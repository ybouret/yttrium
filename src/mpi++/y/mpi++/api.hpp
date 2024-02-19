//! \file

#ifndef Y_MPI_INCLUDED
#define Y_MPI_INCLUDED 1

#include "y/mpi++/cxx.hpp"
#include "y/singleton.hpp"
#include "y/exception.hpp"
#include "y/concurrent/context.hpp"
#include "y/system/rtti.hpp"
#include "y/associative/little-endian-address.hpp"
#include "y/stream/data/output-buffer.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! MPI interface
    //
    //
    //__________________________________________________________________________
    class MPI : public Singleton<MPI>, public  MPIXX, public Concurrent::Context
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const char * const      CallSign;
        static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 20;
        typedef uint64_t               (*GetTicks)(void);
        static  const size_t           MaximumSize = static_cast<size_t>(IntegerFor<int>::Maximum);
        static  const int              DefaultTag  = 0x07;
        typedef OutputBuffer<9>        SizeExchanger;
        static  const uint8_t          SYN = 0x01;

        class Exception : public Yttrium::Exception
        {
        public:
            explicit Exception(const int err, const char *fmt,...) noexcept Y_PRINTF_CHECK(3,4);
            virtual ~Exception() noexcept;
            Exception(const Exception &) noexcept;
            virtual const char * what() const noexcept; //!< return MPI error string

            const int code;
        private:
            Y_DISABLE_ASSIGN(Exception);
            char mesg[MPI_MAX_ERROR_STRING];
        };

        typedef LittleEndianKey DataKey;

        class DataType : public Object, public Counted
        {
        public:
            template <typename T> inline
            DataType( const Type2Type<T>, const MPI_Datatype t) :
            Object(),
            Counted(),
            rtti( RTTI::Of<T>() ),
            type( t ),
            size( sizeof(T) ),
            uuid( rtti )
            {
            }

            ~DataType() noexcept;

            const DataKey & key() const  noexcept;

            const RTTI               &rtti;
            const MPI_Datatype        type;
            const size_t              size;
            const LittleEndianAddress uuid;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DataType);
        };

        class Monitor
        {
        public:
            Monitor()    noexcept;
            ~Monitor()   noexcept;
            Y_OSTREAM_PROTO(Monitor);

            void reset() noexcept;
            void record(const uint64_t n, const uint64_t t) noexcept;


            const uint64_t bytes;
            const uint64_t ticks;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Monitor);
        };

        class Traffic
        {
        public:
            Traffic() noexcept;
            ~Traffic() noexcept;
            Y_OSTREAM_PROTO(Traffic);

            void reset() noexcept;

            Monitor send;
            Monitor recv;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Traffic);
        };



        // management
        static MPI &Init(int *argc, char ***argv, const int thread_support);


        // methods
        const DataType & get(const RTTI &) const;

        // Point to point
        void send(const void * const data,
                  const size_t       count,
                  const DataType    &datatype,
                  const size_t       destination,
                  const int          tag);

        template <typename T> inline
        void send(const T * const entry,
                  const size_t    count,
                  const size_t    destination,
                  const int       tag = DefaultTag)
        {
            static const DataType &datatype = get( RTTI::Of<T>() );
            send(entry,count,datatype,destination,tag);
        }

        void sendSize(const size_t sz,
                      const size_t destination,
                      const int    tag = DefaultTag);


        void recv(void *             data,
                  const size_t       count,
                  const DataType    &datatype,
                  const size_t       source,
                  const int          tag);

        size_t recvSize(const size_t source,
                        const int    tag = DefaultTag);

        template <typename T> inline
        void recv(T *          entry,
                  const size_t count,
                  const size_t source,
                  const int    tag = DefaultTag)
        {
            static const DataType &datatype = get( RTTI::Of<T>() );
            recv(entry,count,datatype,source,tag);
        }

        void primarySyn()
        {
            assert(primary);
            for(size_t i=1;i<size;++i) send(&SYN,1,rank);
        }

        // members
        SizeExchanger      sizeIO;
        const GetTicks     getTicks;
        Traffic            traffic;
        const char * const processorName;
        const bool         parallel;
        const bool         primary;
        const bool         replica;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(MPI);
        friend class Singleton<MPI>;
        explicit MPI();
        virtual ~MPI() noexcept;
    };


#define Y_MPI_CALL(PROCEDURE) do {                      \
/**/    const int res = (PROCEDURE);                    \
/**/    if(MPI_SUCCESS!=res)                            \
/**/         throw MPI::Exception(res,"%s",#PROCEDURE); \
/**/ } while(false)





}

#endif

