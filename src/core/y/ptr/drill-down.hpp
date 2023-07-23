
//! \file

#ifndef Y_Ptr_Drill_Down_Included
#define Y_Ptr_Drill_Down_Included 1

#include "y/type/args.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! DrillDown access with operator->
    //
    //
    //__________________________________________________________________________
    template <class T> class DrillDown
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_EXPOSE(T,Type);               //!< aliases
        typedef Type &      ReturnType;      //!< alias
        typedef ConstType & ConstReturnType; //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        inline virtual ~DrillDown() noexcept {} //!< cleanup
    protected:
        inline explicit DrillDown() noexcept {} //!< setup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline ReturnType      fetch(Type      *ptr)       noexcept { assert(0!=ptr); return *ptr; } //!< proxy for operator->
        inline ConstReturnType fetch(ConstType *ptr) const noexcept { assert(0!=ptr); return *ptr; } //!< proxy for operator->

    private:
        Y_DISABLE_COPY_AND_ASSIGN(DrillDown);
    };

}

#endif

