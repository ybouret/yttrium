//! \file

#ifndef Y_Counting_Partition_Included
#define Y_Counting_Partition_Included 1

#include "y/counting/schedule.hpp"
#include "y/kemp/natural.hpp"

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
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const char * const CallSign;  //!< "Partition"
        static apn Cardinal(const size_t n); //!< from Euler's formula

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit Partition(const size_t n); //!< integer to part
        virtual ~Partition() noexcept;      //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        virtual const char * callSign()               const noexcept; //!< CallSign
        virtual size_t       size()                   const noexcept; //!< current parts
        ConstType &          operator[](const size_t) const noexcept; //!< access parts[1..size()]
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Partition);
        class Code;
        Code *code;

        virtual void doBoot() noexcept;
        virtual bool doNext() noexcept;

    };
    

}

#endif

