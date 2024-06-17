//! \file

#ifndef Y_Duration_Included
#define Y_Duration_Included 1

#include "y/config/starting.hpp"
#include "y/calculus/align.hpp"

#if defined(Y_BSD)
#include <sys/time.h>
#endif

#if defined(Y_WIN)
#include <winsock2.h>
#endif


namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Duration built around timeval
    //
    //
    //__________________________________________________________________________
    class Duration
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
      
        //! Status
        enum Status
        {
            Standard=0, //!< positive or zero time
            Infinite=1  //!< negative time
        };
        static const size_t Required = 4 * sizeof(uint64_t); //!< to store info
        static const size_t MaxWait  = 60;                   //!< max wait in seconds


        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        Duration(const double s)               noexcept; //!< setup
        Duration(const Duration &)             noexcept; //!< copy
        Duration & operator=(const Duration &) noexcept; //!< assign
        Duration & operator=(const double   s) noexcept; //!< assign
        ~Duration()                            noexcept; //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        Status                 status() const noexcept; //!< status
        operator double()               const noexcept; //!< conversion
        struct timeval *       tv()           noexcept; //!< NULL or positive
        const struct timeval * tv()     const noexcept; //!< NULL or positive


    private:
        mutable void *wksp[ Y_WORDS_GEQ(Required) ];
        void ldz() noexcept;
        void ld(double s) noexcept;
        Status &         getStatus() const noexcept;
        struct timeval & getTV()     const noexcept;

    };


}

#endif
