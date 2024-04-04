
//! \file

#ifndef Y_Auto_Ptr_Included
#define Y_Auto_Ptr_Included 1

#include "y/ptr/ptr.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! BarePtr to mimic any pointer
    //
    //
    //__________________________________________________________________________
    template <typename T>
    class BarePtr : public Ptr<T,Immediate>
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef Ptr<T,Immediate> SelfType; //!< alias
        Y_ARGS_EXPOSE(T,Type);             //!< aliases
        using SelfType::handle;            //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        inline          BarePtr( Type *ptr )          noexcept : SelfType(ptr) {}
        inline virtual ~BarePtr()                     noexcept { handle=0; }
        inline          BarePtr(const BarePtr &other) noexcept : SelfType(other.handle) {}
        inline          BarePtr & operator=(const BarePtr &other) noexcept
        {
            handle = other.handle;
            return *this;
        }

    };
}


#endif
