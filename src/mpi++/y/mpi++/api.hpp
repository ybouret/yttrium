//! \file

#ifndef Y_MPI_INCLUDED
#define Y_MPI_INCLUDED 1

#include "y/mpi++/cxx.hpp"
#include "y/singleton.hpp"
#include "y/exception.hpp"
#include "y/concurrent/context.hpp"
#include "y/system/rtti.hpp"
#include "y/associative/little-endian-address.hpp"
#include <cstdio>

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
        static  const int              Tag = 0x07;       //!< if needed
        typedef uint64_t             (*GetTicks)(void);  //!< getting [Locked]Ticks
        static  const size_t           MaxCount = static_cast<size_t>(IntegerFor<int>::Maximum); //!< size_t as int

        //______________________________________________________________________
        //
        //
        // Definitions for singleton
        //
        //______________________________________________________________________
        static const char * const      CallSign;                                 //!< "MPI"
        static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 20; //!< lifetime


        //______________________________________________________________________
        //
        //
        //! Exception
        //
        //______________________________________________________________________
        class Exception : public Yttrium::Exception
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup from MPI error code and context
            explicit Exception(const int err, const char *fmt,...) noexcept Y_PRINTF_CHECK(3,4);
            virtual ~Exception()         noexcept;                                               //!< cleanup
            Exception(const Exception &) noexcept;                                               //!< copy

            //__________________________________________________________________
            //
            // Methods
            //__________________________________________________________________
            virtual const char * what() const noexcept; //!< return MPI error string

            //__________________________________________________________________
            //
            // Members
            //__________________________________________________________________
            const int code; //!< MPI error code
        private:
            Y_DISABLE_ASSIGN(Exception);
            char mesg[MPI_MAX_ERROR_STRING];
        };


        typedef LittleEndianKey DataKey; //!< interface for datatype

        //______________________________________________________________________
        //
        //
        //! Advanced DataType, using RTTI as key
        //
        //______________________________________________________________________
        class DataType : public Object, public Counted
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
           
            //! setup from type and allegded datatype
            template <typename T> inline
            DataType( const Type2Type<T>, const MPI_Datatype t) :
            Object(),
            Counted(),
            rtti( RTTI::Of<T>() ),
            type( t             ),
            size( sizeof(T)     ),
            uuid( rtti          )
            {
            }

            //! cleanup
            ~DataType() noexcept;

            //__________________________________________________________________
            //
            // Methods
            //__________________________________________________________________

            //! return uuid, key from unique rtti
            const DataKey & key() const  noexcept;

            //__________________________________________________________________
            //
            // Members
            //__________________________________________________________________
            const RTTI               &rtti; //!< type of T
            const MPI_Datatype        type; //!< matching data type
            const size_t              size; //!< sizeof(T)
            const LittleEndianAddress uuid; //!< made from &rtti

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DataType);
        };

        //______________________________________________________________________
        //
        //
        //! Traffic Monitor
        //
        //______________________________________________________________________
        class Monitor
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            Monitor()    noexcept;    //!< setup
            ~Monitor()   noexcept;    //!< cleanup
            Y_OSTREAM_PROTO(Monitor); //!< display

            //__________________________________________________________________
            //
            // Methods
            //__________________________________________________________________
            void reset() noexcept;                                    //!< reset all
            void record(const uint64_t n, const uint64_t t) noexcept; //!< record traffic

            //__________________________________________________________________
            //
            // Members
            //__________________________________________________________________
            const uint64_t bytes; //!< total transferred
            const uint64_t ticks; //!< total ticks

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Monitor);
        };

        //______________________________________________________________________
        //
        //
        //! Traffic
        //
        //______________________________________________________________________
        class Traffic
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            Traffic()  noexcept;       //!< setup
            ~Traffic() noexcept;       //!< cleanup
            Y_OSTREAM_PROTO(Traffic);  //!< display

            //__________________________________________________________________
            //
            // Methods
            //__________________________________________________________________
            void reset() noexcept; //!< reset all

            //__________________________________________________________________
            //
            // Members
            //__________________________________________________________________
            Monitor send; //!< traffic for send
            Monitor recv; //!< traffic for recv

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Traffic);
        };


        //______________________________________________________________________
        //
        //
        // Management
        //
        //______________________________________________________________________

        //! setup singleton
        /**
         \param argc from main
         \param argv from main
         \param thread_support required from MPI_THREAD[...]
         */
        static MPI &Init(int *argc, char ***argv, const int thread_support);

        //______________________________________________________________________
        //
        //
        // methods
        //
        //______________________________________________________________________

        //! get advanced datatype from RTTI
        const DataType & get(const RTTI &) const;

        //______________________________________________________________________
        //
        //
        //
        // Point to point interface
        //
        //
        //______________________________________________________________________

        //______________________________________________________________________
        //
        //
        // sending
        //
        //______________________________________________________________________

        //! sending block
        /**
         \param entry       data address
         \param count       number of items
         \param datatype    advanced datatype
         \param destination rank to send to
         \param tag         communication tag
         */
        void send(const void * const entry,
                  const size_t       count,
                  const DataType    &datatype,
                  const size_t       destination,
                  const int          tag);

        //! sending blocks
        /**
         \param blockAddr   first block
         \param numBlocks   number of blocks
         \param destination rank to send to
         \param tag         communication tag
         */
        template <typename T> inline
        void send(const T * const blockAddr,
                  const size_t    numBlocks,
                  const size_t    destination,
                  const int       tag)
        {
            static const DataType &datatype = get( RTTI::Of<T>() );
            send(blockAddr,numBlocks,datatype,destination,tag);
        }



        //______________________________________________________________________
        //
        //! send aggregate of primitive types
        //______________________________________________________________________
        template <typename T, template <typename> class AGG>
        inline void sendND(const AGG<T> * const entry,
                           const size_t         count,
                           const size_t         destination,
                           const int            tag)
        {
            static const size_t    DIMS     = sizeof(AGG<T>)/sizeof(T);
            static const DataType &sendtype = get( RTTI::Of<T>() );
            send(entry,count*DIMS,sendtype,destination,tag);
        }


        //! heterogeneous sendSize
        /**
         use IO::Pack64 to encode
         \param sz          system size
         \param destination rank to send to
         \param tag         communication tag
         */
        void sendSize(const size_t sz,
                      const size_t destination,
                      const int    tag);

        //______________________________________________________________________
        //
        //! send one object, default
        //______________________________________________________________________
        template <typename T>
        struct SendOne
        {
            //! use regular send
            static inline void With(MPI &mpi, const T &obj, const size_t dst, const int tag) {
                mpi.send(&obj,1,dst,tag);
            }
        };



        //______________________________________________________________________
        //
        //
        // recving
        //
        //______________________________________________________________________

        //! receiving block
        /**
         \param entry       data address
         \param count       number of items
         \param datatype    advanced datatype
         \param source      rank to recv from
         \param tag         communication tag
         */
        void recv(void *             entry,
                  const size_t       count,
                  const DataType    &datatype,
                  const size_t       source,
                  const int          tag);

        //! receiving blocks of primtive types
        /**
         \param blockAddr   first block
         \param numBlocks   number of blocks
         \param source      rank to recv from
         \param tag         communication tag
         */
        template <typename T> inline
        void recv(T * const    blockAddr,
                  const size_t numBlocks,
                  const size_t source,
                  const int    tag)
        {
            static const DataType &datatype = get( RTTI::Of<T>() );
            recv(blockAddr,numBlocks,datatype,source,tag);
        }

        //______________________________________________________________________
        //
        //! recv aggregate of primitive types
        //______________________________________________________________________
        template <typename T, template <typename> class AGG>
        inline void recvND(AGG<T> * const entry,
                           const size_t   count,
                           const size_t   source,
                           const int      tag)
        {
            static const size_t    DIMS     = sizeof(AGG<T>)/sizeof(T);
            static const DataType &recvtype = get( RTTI::Of<T>() );
            recv(entry,count*DIMS,recvtype,source,tag);
        }
        

        //! heterogeneous recvSize
        /**
         use IO::Pack64 to decode data
         \param source      rank to recv from
         \param tag         communication tag
         */
        size_t recvSize(const size_t source,
                        const int    tag);


        //______________________________________________________________________
        //
        //! recv one object, default
        //______________________________________________________________________
        template <typename T>
        struct RecvOne
        {
            //! use regular recv
            static inline T With(MPI &mpi, const size_t src, const int tag )
            {
                T res;
                mpi.recv(&res, 1, src, tag);
                return res;
            }
        };

        //______________________________________________________________________
        //
        //
        // Send/Recv
        //
        //______________________________________________________________________

        //! sendrecv
        void sendrecv(const void * const sendbuf,
                      const size_t       sendcount,
                      const DataType    &sendtype,
                      const size_t       destination,
                      const int          sendtag,
                      void * const       recvbuf,
                      const size_t       recvcount,
                      const DataType    &recvtype,
                      const size_t       source,
                      const int          recvtag);

        //! sendrecv of primitive types
        template <typename T, typename U> inline
        void sendrecv(const T *  const sendEntry,
                      const size_t     sendCount,
                      const size_t     destination,
                      const int        sendTag,
                      U * const        recvEntry,
                      const size_t     recvCount,
                      const size_t     source,
                      const int        recvTag)
        {
            static const DataType &sendtype = get( RTTI::Of<T>() );
            static const DataType &recvtype = get( RTTI::Of<U>() );
            sendrecv(sendEntry, sendCount, sendtype, destination, sendTag,
                     recvEntry, recvCount, recvtype, source,      recvTag);
        }

        //______________________________________________________________________
        //
        //
        // Helpers
        //
        //______________________________________________________________________
        void syn(const size_t destination); //!< send a sync message to destination
        void ack(const size_t source);      //!< recv a sync message from source
        void synNext();                     //!< next rank, if parallel and not closing
        void ackPrev();                     //!< prev rank, if parallel and not leading

        //! local check same value than args
        template <typename T> inline
        bool isEqualToPrimary(const T &args)
        {
            if(primary)
            {
                bool result = true;
                for(size_t r=1;r<size;++r)
                    send(&args, 1, r, Tag);
                return result;
            }
            else
            {
                T temp(0);
                recv(&temp,1,0,Tag);
                return temp == args;
            }
        }


        //______________________________________________________________________
        //
        //! print formatted output to C FILE on primart
        /**
         \param fp FILE on primary only
         \param fmt printf-like format
         */
        //______________________________________________________________________
        void print(FILE *fp, const char *fmt,...) Y_PRINTF_CHECK(3,4);

        //______________________________________________________________________
        //
        //
        // Collective interface
        //
        //______________________________________________________________________

        //! Bcast
        void broadcast(void * const    entry,
                       const size_t    count,
                       const DataType &datatype,
                       const size_t    from);

        //! Bcast primitive type
        template <typename T> inline
        void broadcast(T * const    blockAddr,
                       const size_t numBlocks,
                       const size_t from)
        {
            static const DataType &datatype = get( RTTI::Of<T>() );
            broadcast(blockAddr, numBlocks,datatype,from);
        }

        //! Bcast aggregate of primitive type(s)
        template <typename T, template <typename> class AGG> inline
        void broadcastND(AGG<T> * const entry,
                         const size_t   count,
                         const size_t   from)
        {
            static const size_t    DIMS = sizeof(AGG<T>)/sizeof(T);
            static const DataType &type = get( RTTI::Of<T>() );
            broadcast(entry,count*DIMS,type,from);

        }

        //______________________________________________________________________
        //
        //! print formatted output to C FILE on primary
        //______________________________________________________________________
        template <typename T>
        struct CastOne
        {
            //! use regular broadcast
            static inline void With(MPI &mpi, T &args, const size_t root)
            {
                static const T _0(0);
                const  bool    isRoot = (root == mpi.rank);
                T res( isRoot ? args : _0 );
                mpi.broadcast(&res, 1, root);
                args = res;
            }
        };



        //______________________________________________________________________
        //
        //
        // Members
        //
        //______________________________________________________________________
        const GetTicks     getTicks;        //!< ticks protocol
        Traffic            traffic;         //!< current traffic
        const char * const processorName;   //!< processor name
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(MPI);
        friend class Singleton<MPI>;
        explicit MPI();
        virtual ~MPI() noexcept;
    };

    //__________________________________________________________________________
    //
    //! send one string
    //__________________________________________________________________________
    template <> struct MPI:: SendOne<String>
    {
        //! send size + data
        static void With(MPI &, const String &, const size_t, const int);
    };


    //__________________________________________________________________________
    //
    //! recv one string
    //__________________________________________________________________________
    template <> struct MPI:: RecvOne<String>
    {
        //! recv size + data
        static String With(MPI &, const size_t, const int);
    };

    //__________________________________________________________________________
    //
    //
    //! retrieve result and throw upon failure
    //
    //__________________________________________________________________________
#define Y_MPI_CALL(PROCEDURE) do {                      \
/**/    const int res = (PROCEDURE);                    \
/**/    if(MPI_SUCCESS!=res)                            \
/**/         throw MPI::Exception(res,"%s",#PROCEDURE); \
/**/ } while(false)

    //__________________________________________________________________________
    //
    //
    //! perform in order operations
    //
    //__________________________________________________________________________
#define Y_MPI_SEQUENTIAL(THE_MPI,CODE)  \
/**/    do { \
/**/        switch(THE_MPI.ppty) { \
/**/          case Yttrium::Concurrent::OnlyOne:                    do { CODE; } while(false);                    break; \
/**/          case Yttrium::Concurrent::Leading:                    do { CODE; } while(false); THE_MPI.synNext(); break; \
/**/          case Yttrium::Concurrent::Generic: THE_MPI.ackPrev(); do { CODE; } while(false); THE_MPI.synNext(); break; \
/**/          case Yttrium::Concurrent::Closing: THE_MPI.ackPrev(); do { CODE; } while(false);                    break; \
/**/        }\
/**/    } while(false)




}

#endif

