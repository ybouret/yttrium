
//! \file

#ifndef Y_Sequence_Vector_Included
#define Y_Sequence_Vector_Included 1

#include "y/sequence/interface.hpp"
#include "y/container/dynamic.hpp"
#include "y/object.hpp"
#include "y/memory/wad.hpp"
#include "y/container/operating.hpp"
#include "y/memory/allocator/global.hpp"

namespace Yttrium
{

    namespace Core
    {
        class Vector
        {
        public: static const char * const CallSign; //!< "Vector"
        protected: explicit Vector() noexcept;
        public:    virtual ~Vector() noexcept;
        private:   Y_DISABLE_COPY_AND_ASSIGN(Vector);
        };
    }

    template <typename T, typename ALLOCATOR = Memory::Global>
    class Vector : public Dynamic, public Sequence<T>, public Core::Vector
    {
    public:
        Y_ARGS_DECL(T,Type);

        explicit Vector() noexcept: code( 0 ) {}

        virtual ~Vector() noexcept { release(); }

        inline virtual const char * callSign() const noexcept { return CallSign; }
        inline virtual size_t       size()     const noexcept { return code ? code->size     :  0; }
        inline virtual size_t       capacity() const noexcept { return code ? code->maxBlocks : 0; }
        inline virtual void         free()           noexcept { if(0!=code) code->free(); }
        inline virtual void         release()        noexcept { if(0!=code) { delete code; code=0; } }
        inline virtual void         reserve(const size_t n)
        {
            if(n<=0) return;
            if(0!=code)
            {
                Code *temp = new Code(code->maxBlocks+n);
                Memory::OutOfReach::Grab(temp->base,code->base,code->size*sizeof(T));
                code->size = 0;
                delete code;
                code = temp;
            }
            else
            {
                code = new Code(n);
            }
        }

        inline virtual void popTail() noexcept
        {
            assert(0!=code);
            assert(code->size>0);
            code->popTail();
        }

        inline virtual void popHead() noexcept
        {
            assert(0!=code);
            assert(code->size>0);
            code->popHead();
        }


    private:
        class Code;
        Code *code;

        class Code : public Object, public Memory::Wad<Type,ALLOCATOR>
        {
        public:
            typedef Memory::Wad<Type,ALLOCATOR> WadType;
            typedef Memory::OutOfReach          MemOps;

            inline explicit Code(const size_t n) : Object(), WadType(n),
            base(static_cast<MutableType*>(this->workspace)),
            item(base-1),
            size(0)
            {
                assert(this->maxBlocks>=n);
            }

            inline virtual ~Code() noexcept {}
            inline void     free()    noexcept { while(size>0) popTail(); }

            inline void     popTail() noexcept { assert(size>0); MemOps::Naught( base[--size] ); }
            inline void     popHead() noexcept { assert(size>0); MemOps::Grab( MemOps::Naught(base),base+1,(--size)*sizeof(T)); }



            MutableType *base;
            MutableType *item;
            size_t       size;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };
    };
}

#endif
