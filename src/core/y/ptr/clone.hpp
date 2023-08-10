//! \file

#ifndef Y_Clone_Ptr_Included
#define Y_Clone_Ptr_Included 1

#include "y/ptr/ptr.hpp"

namespace Yttrium
{

    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //! Common behaviour for ClonePtr
        //
        //______________________________________________________________________
        class ClonePtr
        {
        protected: explicit ClonePtr() noexcept; //!< setup
        public:    virtual ~ClonePtr() noexcept; //!< cleanup
            static const char * const CallSign;  //!< "ClonePtr"
            static void NoNull(void *) noexcept; //!< critical error

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClonePtr);
        };
    }

    //__________________________________________________________________________
    //
    //
    //
    //! AutoPtr to manage clone
    //
    //
    //__________________________________________________________________________
    template <typename T, template <typename> class Policy = Immediate >
    class ClonePtr : public Core::ClonePtr, public Ptr<T,Policy>
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
        inline explicit ClonePtr(Type *ptr) noexcept :
        Core::ClonePtr(), SelfType(ptr) { NoNull(handle); }

        //______________________________________________________________________
        //
        //! cleanup by dimissing
        //______________________________________________________________________
        inline virtual ~ClonePtr() noexcept {  assert(0!=handle); delete handle; handle=0; }

        //______________________________________________________________________
        //
        //! copy, transfer ownership
        //______________________________________________________________________
        inline ClonePtr(const ClonePtr &other) noexcept :
        Core::ClonePtr(), SelfType(other.handle->clone()) { NoNull(handle); }

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! exchange content
        inline void xch(ClonePtr &other) noexcept
        {
            Swap(handle,other.handle);
        }

    private:
        Y_DISABLE_ASSIGN(ClonePtr);



    };

}

#endif

