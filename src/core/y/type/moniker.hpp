//! \file

#ifndef Y_Type_Moniker_Included
#define Y_Type_Moniker_Included 1

#include "y/type/args.hpp"
#include "y/calculus/align.hpp"
#include "y/memory/out-of-reach.hpp"
#include <iostream>

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! workaround for misaligned type
    //
    //
    //______________________________________________________________________
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
        inline Moniker(ParamType src) : addr(0), wksp() 
        {
            new ( link() )  MutableType(src);
        }


        //! copy using type copy constructor
        inline Moniker(const Moniker &other) : addr(0), wksp() { new ( link() )  MutableType(*other); }


        //! deep clean
        inline ~Moniker() noexcept { Memory::OutOfReach::Naught(addr); addr=0; }

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
        MutableType *addr;
        void        *wksp[ Y_WORDS_FOR(T) ];

        inline void *link()  
        {
            try { return (addr = static_cast<MutableType *>( Y_STATIC_ZARR(wksp) )); }
            catch(...) { addr = 0; throw; }
        }

    };



}

#endif

