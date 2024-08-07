//! \file

#ifndef Y_Ptr_Included
#define Y_Ptr_Included 1

#include "y/ptr/core.hpp"
#include "y/ptr/immediate.hpp"
#include <iostream>

namespace Yttrium
{


    //__________________________________________________________________________
    //
    //
    //
    //! Base class for smart pointers, holding handle
    //
    //
    //__________________________________________________________________________
    template <typename T, template <class> class Policy>
    class Ptr : public Core::Ptr, public Policy<T>
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef typename Policy<T>::ReturnType      ReturnType;      //!< alias
        typedef typename Policy<T>::ConstReturnType ConstReturnType; //!< alias
        Y_ARGS_EXPOSE(T,Type);                                       //!< aliases

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
    protected:
        inline explicit Ptr(Type *ptr) noexcept : handle( (MutableType*)ptr ) {} //!< setup
    public:
        inline virtual ~Ptr()           noexcept { assert(0==handle); }          //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline bool     isValid() const noexcept { return 0!=handle;  } //!< check validity
        inline bool     isEmpty() const noexcept { return 0==handle;  } //!< check dangling

        inline Type      &     operator*()        noexcept { assert(isValid()); return *handle; }             //!< dereference
        inline ConstType &     operator*()  const noexcept { assert(isValid()); return *handle; }             //!< dereference, const
        inline ReturnType      operator->()       noexcept { assert(isValid()); return this->fetch(handle); } //!< access w.r.t. policy
        inline ConstReturnType operator->() const noexcept { assert(isValid()); return this->fetch(handle); } //!< access w.r.t. policy, const

        //! forwarding display function
        inline friend std::ostream & operator<<(std::ostream &os, const Ptr &self)
        {
            if(self.handle) os << (*self.handle);
            else            os << Nil;
            return os;
        }
        
        //! get anonymous address
        inline const void * address() const noexcept { return handle; }

    protected:
        MutableType *handle; //!< handle for all smart pointers

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Ptr);
    };



}

#endif

