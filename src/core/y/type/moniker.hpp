//! \file

#ifndef Y_Type_Moniker_Included
#define Y_Type_Moniker_Included 1

#include "y/type/args.hpp"
#include "y/calculus/align.hpp"
#include "y/memory/out-of-reach.hpp"
#include <iostream>

namespace Yttrium
{

    //! workaround for misaligned type
    template <typename T>
    class Moniker
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_DECL(T,Type); //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! setup from source
        inline Moniker(ParamType src) :
        wksp(), addr( Memory::OutOfReach::Cast<MutableType>(wksp) )
        { new ( Y_STATIC_ZARR(wksp) ) MutableType(src); }


        //! copy
        inline Moniker(const Moniker &other) :
        wksp(), addr( Memory::OutOfReach::Cast<MutableType>(wksp) )
        { new ( Y_STATIC_ZARR(wksp) ) MutableType(*other); }


        //! deep clean
        inline ~Moniker() noexcept
        { Memory::OutOfReach::Naught(addr); addr=0; }

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        inline Type      & operator*()       noexcept { return *addr; } //!< access
        inline ConstType & operator*() const noexcept { return *addr;}  //!< access

        inline Type *      operator->()       noexcept { return addr; } //!< access
        inline ConstType * operator->() const noexcept { return addr; } //!< access

        //! display
        inline friend std::ostream & operator<<(std::ostream &os, const Moniker &self)
        { return os << *self; }

    private:
        Y_DISABLE_ASSIGN(Moniker);
        void        *wksp[ Y_WORDS_FOR(T) ];
        MutableType *addr;

    };



}

#endif

