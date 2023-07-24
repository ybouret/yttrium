
//! \file

#ifndef Y_Sequence_Vector_Included
#define Y_Sequence_Vector_Included 1

#include "y/sequence/interface.hpp"
#include "y/container/dynamic.hpp"
#include "y/container/writable.hpp"
#include "y/object.hpp"
#include "y/memory/wad.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/memory/workspace.hpp"

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
    class Vector :
    public Dynamic,
    public Sequence<T>,
    public Core::Vector,
    public Writable<T>
    {
    public:
        Y_ARGS_DECL(T,Type);
        typedef Memory::Workspace<MutableType> Workspace;
        typedef Memory::OutOfReach          MemOps;

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
            minimalCapacity(code->maxBlocks+n);
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

        inline virtual void pushHead(ParamType args)
        {
            Workspace  data;
            ConstType &temp = data.build(args);
            checkSpace();
            assert(0!=code);
            code->pushHead(temp);
            data.dismiss();
        }

        inline virtual void pushTail(ParamType args)
        {
            Workspace data;
            ConstType &temp = data.build(args);
            checkSpace();
            assert(0!=code);
            code->pushTail(temp);
            data.dismiss();
        }

        inline virtual Type & operator[](const size_t indx) noexcept
        {
            assert(indx>=1);
            assert(indx<=size());
            assert(0!=code);
            return code->item[indx];
        }

        inline virtual ConstType & operator[](const size_t indx) const noexcept
        {
            assert(indx>=1);
            assert(indx<=size());
            assert(0!=code);
            return code->item[indx];
        }




    private:
        class Code;
        Code *code;

        inline void minimalCapacity(const size_t n)
        {
            if(0==code)
            {
                code = new Code( n );
            }
            else
            {
                assert(n>code->maxBlocks);
                Code *temp = new Code(n);
                Memory::OutOfReach::Grab(temp->base,code->base,code->size*sizeof(T));
                code->size = 0;
                delete code;
                code = temp;
            }
        }

        inline void checkSpace()
        {
            const size_t capa = capacity();
            if(size()>=capa)
                minimalCapacity( NextCapacity(capa) );
            assert(0!=code);
            assert(code->size<code->maxBlocks);
        }

        class Code : public Object, public Memory::Wad<Type,ALLOCATOR>
        {
        public:
            typedef Memory::Wad<Type,ALLOCATOR> WadType;

            inline explicit Code(const size_t n) : Object(), WadType(n),
            base(static_cast<MutableType*>(this->workspace)),
            item(base-1),
            size(0)
            {
                assert(this->maxBlocks>=n);
            }

            inline virtual ~Code() noexcept { free(); }
            inline void     free() noexcept { while(size>0) popTail(); }

            inline void     popTail() noexcept { assert(size>0); MemOps::Naught( &base[--size] ); }
            inline void     popHead() noexcept { assert(size>0);
                MemOps::Move( MemOps::Naught(base),base+1,(--size)*sizeof(T));
                MemOps::Zero(base+size,sizeof(T));
            }

            inline void pushTail(ConstType &temp) noexcept
            {
                assert(size<this->maxBlocks);
                MemOps::Copy(&base[size++],&temp,sizeof(T));
            }

            inline void pushHead(ConstType &temp) noexcept
            {
                assert(size<this->maxBlocks);
                MemOps::Move(base+1,base,size*sizeof(T));
                MemOps::Copy(base,&temp,sizeof(T));
                ++size;
            }


            MutableType * const base;
            MutableType * const item;
            size_t              size;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };
    };
}

#endif
