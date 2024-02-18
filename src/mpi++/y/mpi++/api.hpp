//! \file

#ifndef Y_MPI_INCLUDED
#define Y_MPI_INCLUDED 1

#include "y/mpi++/cxx.hpp"
#include "y/singleton.hpp"
#include "y/exception.hpp"
#include "y/concurrent/context.hpp"

namespace Yttrium
{
    
    class MPI : public Singleton<MPI>, public  MPIXX, public Concurrent::Context
    {
    public:
        static const char * const      CallSign;
        static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 20;

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

        // management
        static MPI &Init(int *argc, char ***argv);

        // members
        const char * const processorName;

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

