
//! \file

#ifndef Y_Auto_Ptr_Included
#define Y_Auto_Ptr_Included 1

#include "y/ptr/ptr.hpp"

namespace Yttrium
{

    namespace Core
    {
        class AutoPtr
        {
        protected: explicit AutoPtr() noexcept;
        public:    virtual ~AutoPtr() noexcept;
            static const char * const CallSign;
            void   Critical(const void *) noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(AutoPtr);
        };
    }

    template <typename T, template <typename> class Policy = Immediate >
    class AutoPtr : public Core::AutoPtr, public Ptr<T,Policy>
    {
    public:
        typedef Ptr<T,Policy> SelfType;
        Y_ARGS_EXPOSE(T);
        using SelfType::handle;

        inline          AutoPtr(Type *ptr) noexcept : Core::AutoPtr(),SelfType(ptr) { }
        inline virtual ~AutoPtr()          noexcept { dismiss(); }
        inline AutoPtr(const AutoPtr &other) noexcept :
        Core::AutoPtr(), SelfType(other.handle)
        {
            Coerce(other.handle) = 0;
        }

        inline AutoPtr & operator=(const AutoPtr &other) noexcept
        {
            if(this!=other)
            {
                if(0!=handle) delete handle;
                handle = other.handle;
                Coerce(other.handle) = 0;
            }
            return *this;
        }

        inline AutoPtr & operator=(Type *ptr) noexcept
        {
            if(ptr != handle)
            {
                if(0!=handle) delete handle;
                handle = (MutableType *)ptr;
            }
            return *this;
        }

        inline void   dismiss() noexcept { if(0!=handle) { delete handle; handle=0; } }

        inline Type * yield() noexcept {
            Type *ptr = handle;
        }

    };

}

#endif

