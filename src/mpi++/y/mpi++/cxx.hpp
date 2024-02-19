
//! \file

#ifndef Y_MPIXX_INCLUDED
#define Y_MPIXX_INCLUDED 1

//! disable mpi c++
#define OMPI_SKIP_MPICXX 1
#include <mpi.h>

#include "y/config/starting.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! MPI initializer
    //
    //
    //__________________________________________________________________________
    class MPIXX
    {
    public:
        //______________________________________________________________________
        //
        //
        // Helpers
        //
        //______________________________________________________________________
        static const char * ThreadSupport(const int) noexcept;  //!< int to human readable thread support
        const char *        threadSupportText() const noexcept; //!< my  human readable thread support


        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        virtual ~MPIXX() noexcept; //!< MPI_Finalize()

    protected:
        explicit MPIXX();              //!< MPI_Init_threed

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        size_t      Comm_size() const;      //!< MPI_Comm_size(MPI_COMM_WORLD)
        size_t      Comm_rank() const;      //!< MPI_Comm_size(MPI_COMM_WORLD)
        const char *processor_name() const; //!< MPI_Get_processor_name()
    private:
        Y_DISABLE_COPY_AND_ASSIGN(MPIXX);

    public:
        const int threadSupport; //!< one of the possible MPI thread support

    };
}

#endif

