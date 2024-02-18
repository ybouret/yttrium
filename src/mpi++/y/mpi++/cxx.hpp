
//! \file

#ifndef Y_MPIXX_INCLUDED
#define Y_MPIXX_INCLUDED 1

#define OMPI_SKIP_MPICXX 1
#include <mpi.h>

#include "y/config/starting.hpp"

namespace Yttrium
{

    class MPIXX
    {
    public:
        static const char * ThreadSupport(const int) noexcept;
        const char *        threadSupportText() const noexcept;

        virtual ~MPIXX() noexcept;

    protected:
        explicit MPIXX();

        size_t      Comm_size() const;
        size_t      Comm_rank() const;
        const char *processor_name() const;
    private:
        Y_DISABLE_COPY_AND_ASSIGN(MPIXX);

    public:
        const int threadSupport;

    };
}

#endif

