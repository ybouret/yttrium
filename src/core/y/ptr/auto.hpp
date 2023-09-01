
//! \file

#ifndef Y_Auto_Ptr_Included
#define Y_Auto_Ptr_Included 1

#include "y/ptr/ptr.hpp"

namespace Yttrium
{

    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //! Common behaviour for AutoPtr
        //
        //______________________________________________________________________
        class AutoPtr
        {
        protected: explicit AutoPtr() noexcept; //!< setup
        public:    virtual ~AutoPtr() noexcept; //!< cleanup
            static const char * const CallSign; //!< "AutoPtr"
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(AutoPtr);
        };
    }

    //__________________________________________________________________________
    //
    //
    //
    //! AutoPtr to manage a unique pointer
    //
    //
    //__________________________________________________________________________
    template <typename T, template <typename> class Policy = Immediate >
    class AutoPtr : public Core::AutoPtr, public Ptr<T,Policy>
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef Ptr<T,Policy> SelfType; //!< alias
        Y_ARGS_EXPOSE(T,Type);          //!< aliases
        using SelfType::handle;         //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //______________________________________________________________________
        //
        //! setup, accept any pointer
        //______________________________________________________________________
        inline          AutoPtr(Type *ptr) noexcept : Core::AutoPtr(),SelfType(ptr) { }

        //______________________________________________________________________
        //
        //! cleanup by dimissing
        //______________________________________________________________________
        inline virtual ~AutoPtr()          noexcept { dismiss(); }

        //______________________________________________________________________
        //
        //! copy, transfer ownership
        //______________________________________________________________________
        inline AutoPtr(const AutoPtr &other) noexcept :
        Core::AutoPtr(), SelfType(other.handle)
        {
            Coerce(other.handle) = 0;
        }

        //______________________________________________________________________
        //
        //! assign with ownership checking
        //______________________________________________________________________
        inline AutoPtr & operator=(const AutoPtr &other) noexcept
        {
            if(this != &other)
            {
                if(0!=handle) delete handle;
                handle = other.handle;
                Coerce(other.handle) = 0;
            }
            return *this;
        }

        //______________________________________________________________________
        //
        //! assign a new pointer with ownership checking
        //______________________________________________________________________
        inline AutoPtr & operator=(Type *ptr) noexcept
        {
            if(ptr != handle)
            {
                if(0!=handle) delete handle;
                handle = (MutableType *)ptr;
            }
            return *this;
        }

        //______________________________________________________________________
        //
        //! exchange contents
        //______________________________________________________________________
        inline void xch(AutoPtr<T> &other) noexcept
        {
            Swap(handle,other.handle);
        }

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline void   dismiss()       noexcept { if(0!=handle) { delete handle; handle=0; }   } //!< dismiss pointee
        inline Type * yield()         noexcept { Type *ptr = handle; handle = 0; return ptr;  } //!< yield pointee
        
    };

}

#endif

