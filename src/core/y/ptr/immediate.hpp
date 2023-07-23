//! \file

#ifndef Y_Ptr_Immediate_Included
#define Y_Ptr_Immediate_Included 1

#include "y/type/args.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Immediate access with operator->
    //
    //
    //__________________________________________________________________________
    template <class T> class Immediate
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_EXPOSE(T,Type);               //!< aliases
        typedef Type *      ReturnType;      //!< alias
        typedef ConstType * ConstReturnType; //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        inline virtual ~Immediate() noexcept {} //!< cleanup
    protected:
        inline explicit Immediate() noexcept {} //!< setup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline ReturnType      fetch(Type       *ptr)       noexcept { assert(0!=ptr); return ptr; } //!< proxy for operator->
        inline ConstReturnType fetch(const Type *ptr) const noexcept { assert(0!=ptr); return ptr; } //!< proxy for operator->

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Immediate);
    };
    
}

#endif

