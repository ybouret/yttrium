//! \file

#ifndef Y_MPI_INCLUDED
#define Y_MPI_INCLUDED 1

#define OMPI_SKIP_MPICXX 1
#include <mpi.h>
#include "y/singleton.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
 
    class MPI : public Singleton<MPI>
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
        

    private:
        Y_DISABLE_COPY_AND_ASSIGN(MPI);
        friend class Singleton<MPI>;
        virtual ~MPI() noexcept;

    };

}

#endif

