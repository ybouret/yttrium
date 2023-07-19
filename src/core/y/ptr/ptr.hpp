//! \file

#ifndef Y_Ptr_Included
#define Y_Ptr_Included 1

#include "y/type/args.hpp"

namespace Yttrium
{

    namespace Core
    {
        class Ptr
        {
        public:    static const char Nil[];
        protected: explicit Ptr() noexcept;
        public:    virtual ~Ptr() noexcept;
            static void Critical(const void *, const char * const) noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Ptr);
        };
    }

    template <class T> class Immediate
    {
    public:
        Y_ARGS_EXPOSE(T);
        typedef Type *      ReturnType;
        typedef ConstType * ConstReturnType;

        inline virtual ~Immediate() noexcept {}

    protected:
        inline explicit Immediate() noexcept {}
        inline ReturnType      fetch(Type       *ptr)       noexcept { assert(0!=ptr); return ptr; }
        inline ConstReturnType fetch(const Type *ptr) const noexcept { assert(0!=ptr); return ptr; }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Immediate);
    };

    template <class T> class DrillDown
    {
    public:
        Y_ARGS_EXPOSE(T);
        typedef Type &      ReturnType;
        typedef ConstType & ConstReturnType;

        inline virtual ~DrillDown() noexcept {}


    protected:
        inline explicit DrillDown() noexcept {}
        inline ReturnType      fetch(Type      *ptr)       noexcept { assert(0!=ptr); return *ptr; }
        inline ConstReturnType fetch(ConstType *ptr) const noexcept { assert(0!=ptr); return *ptr; }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(DrillDown);
    };

    template <typename T, template <class> class Policy>
    class Ptr : public Core::Ptr, public Policy<T>
    {
    public:
        Y_ARGS_EXPOSE(T);
        
    protected:
        inline explicit Ptr(Type *ptr) noexcept : handle( (MutableType*)ptr ) {}

    public:
        inline virtual ~Ptr()     noexcept { assert(0==handle); }

        inline bool     isValid() noexcept { return 0!=handle;  }
        inline bool     isEmpty() noexcept { return 0==handle;  }

        inline Type      & operator*()       noexcept { assert(isValid()); return *handle; }
        inline ConstType & operator*() const noexcept { assert(isValid()); return *handle; }
        
    protected:
        MutableType *handle;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Ptr);
    };



}

#endif

