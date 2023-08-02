
//! \file

#ifndef Y_Ark_Ptr_Included
#define Y_Ark_Ptr_Included 1

#include "y/ptr/ptr.hpp"

namespace Yttrium
{

    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //! Common behaviour for ArkPtr
        //
        //______________________________________________________________________
        class ArkPtr
        {
        protected: explicit ArkPtr() noexcept;   //!< setup
        public:    virtual ~ArkPtr() noexcept;   //!< cleanup
            static const char * const CallSign;  //!< "ArcPtr"
            static void NoNull(void *) noexcept; //!< critical error

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ArkPtr);
        };
    }

    //__________________________________________________________________________
    //
    //
    //
    //! ArkPtr with a Counted API
    //
    //
    //__________________________________________________________________________
    template <typename KEY, typename T, template <typename> class Policy = Immediate >
    class ArkPtr : public Core::ArkPtr, public Ptr<T,Policy>
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
        Y_ARGS_EXPOSE(KEY,Key);         //!< aliases
        using SelfType::handle;         //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //______________________________________________________________________
        //
        //! setup with a valid pointer, increasing refcount
        //______________________________________________________________________
        inline ArkPtr(Type *ptr) noexcept : Core::ArkPtr(), SelfType(ptr)
        {
            NoNull(handle);
            assert(0!=handle);
            handle->withhold();
        }

        //______________________________________________________________________
        //
        //! cleanup
        //______________________________________________________________________
        inline virtual ~ArkPtr() noexcept
        {
            assert(handle);
            if(handle->liberate()) { delete handle;}
            handle=0;
        }

        //______________________________________________________________________
        //
        //! copy handle and increase refcount
        //______________________________________________________________________
        inline ArkPtr(const ArkPtr &other) noexcept :
        Core::ArkPtr(),
        SelfType(other.handle)
        {
            assert(0!=handle || Die("invalid other"));
            handle->withhold();
        }

        //______________________________________________________________________
        //
        //! assign another
        //______________________________________________________________________
        ArkPtr & operator=(const ArkPtr &other) noexcept
        {
            ArkPtr temp(other);
            if(handle->liberate()) delete handle;
            (handle = temp.handle)->withhold();
            return *this;
        }

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! forward key()
        inline ConstKey & key() const noexcept
        {
            return handle->key();
        }

    };

}

#endif

