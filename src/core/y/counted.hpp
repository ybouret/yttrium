
//! \file

#ifndef Y_Counted_Included
#define Y_Counted_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Simplest Counted class for counting intrusive pointers
    //
    //
    //__________________________________________________________________________
    class Counted
    {
    public:
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit Counted() noexcept; //!< setup
        virtual ~Counted() noexcept; //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        void   withhold() noexcept;         //!< ++nref
        bool   liberate() noexcept;         //!< --nref<=0
        size_t quantity() const noexcept;   //!< nref

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Counted);
        size_t nref;
    };
};

#endif
