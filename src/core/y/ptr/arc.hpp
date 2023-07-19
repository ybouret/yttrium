
//! \file

#ifndef Y_Arc_Ptr_Included
#define Y_Arc_Ptr_Included 1

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
        class ArcPtr
        {
        protected: explicit ArcPtr() noexcept; //!< setup
        public:    virtual ~ArcPtr() noexcept; //!< cleanup
            static const char * const CallSign; //!< "ArcPtr"
            static void NoNull(void *) noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ArcPtr);
        };
    }

    //__________________________________________________________________________
    //
    //
    //
    //! ArcPtr with a Counted API
    //
    //
    //__________________________________________________________________________
    template <typename T, template <typename> class Policy = Immediate >
    class ArcPtr : public Core::ArcPtr, public Ptr<T,Policy>
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef Ptr<T,Policy> SelfType; //!< alias
        Y_ARGS_EXPOSE(T);               //!< aliases
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
        inline          ArcPtr(Type *ptr) noexcept : Core::ArcPtr(), SelfType(ptr)
        {
            NoNull(handle);
            assert(0!=handle);
            handle->withhold();
        }

        //______________________________________________________________________
        //
        //! cleanup
        //______________________________________________________________________
        inline virtual ~ArcPtr() noexcept
        {
            assert(handle);
            if(handle->liberate()) { delete handle;}
            handle=0;
        }

        //______________________________________________________________________
        //
        //! copy handle and increase refcount
        //______________________________________________________________________
        inline ArcPtr(const ArcPtr &other) noexcept :
        Core::ArcPtr(),
        SelfType(other.handle)
        {
            assert(0!=handle || Die("invalid other"));
            handle->withhold();
        }

        //______________________________________________________________________
        //
        //! assign another
        //______________________________________________________________________
        ArcPtr & operator=(const ArcPtr &other) noexcept
        {
            ArcPtr temp(other);
            if(handle->liberate()) delete handle;
            (handle = temp.handle)->withhold();
            return *this;
        }

    };

}

#endif

