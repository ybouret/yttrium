//! \file

#ifndef Y_Counting_Partition_Included
#define Y_Counting_Partition_Included 1

#include "y/counting/schedule.hpp"
#include "y/apex/natural.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Partition of n
    //
    //
    //__________________________________________________________________________
    class Partition : public Schedule
    {
    public:
        static const char * const CallSign;  //!< "Partition"
        static apn Cardinal(const size_t n); //!< from Euler's formula

        explicit Partition(const size_t n);
        virtual ~Partition() noexcept;

        virtual const char * callSign()               const noexcept;
        virtual size_t       size()                   const noexcept;
        ConstType &          operator[](const size_t) const noexcept;
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Partition);
        class Code;
        Code *code;

        virtual void doBoot() noexcept;
        virtual bool doNext() noexcept;

    };
    

}

#endif

