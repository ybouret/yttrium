//! \file

#ifndef Y_Ordered_Vector_Included
#define Y_Ordered_Vector_Included 1

#include "y/ordered/orderly.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/comparison.hpp"
#include "y/container/readable.hpp"
#include "y/memory/wad.hpp"
#include "y/container/dynamic.hpp"
#include "y/object.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Core
    {
        class OrderedVector
        {
        public:
            static const char * const CallSign; //!< "OrderedVector"
            explicit OrderedVector() noexcept;  //!< setup
            virtual ~OrderedVector() noexcept;  //!< cleanup
        private:
            Y_DISABLE_COPY_AND_ASSIGN(OrderedVector);
        };
    }


    template <
    typename T,
    typename COMPARATOR = IncreasingComparator,
    typename ALLOCATOR  = Memory::Pooled>
    class OrderedVector : public Dynamic, public Readable<T>, public Core::OrderedVector
    {
    public:
        Y_ARGS_DECL(T,Type);
        explicit OrderedVector() noexcept      : code(0) {}
        explicit OrderedVector(const size_t n) : code( n>0 ? new Code(n) :0 ) {}
        virtual ~OrderedVector() noexcept { release_(); }
        inline   OrderedVector(const OrderedVector &_) : code( Duplicate(_.code) ) {}


        inline virtual const char * callSign() const noexcept { return CallSign; }
        inline virtual void         release()        noexcept { release_(); }
        inline virtual size_t       size()     const noexcept { return 0!=code ? code->size : 0; }
        inline virtual size_t       capacity() const noexcept { return 0!=code ? code->maxBlocks : 0; }
        inline virtual void         free()           noexcept { if(0!=code) code->free();        }
        inline virtual ConstType & operator[](const size_t indx) const noexcept
        {
            assert(0!=code);
            assert(indx>=1);
            assert(indx<=size());
            return code->data[indx];
        }

        inline virtual void reserve(size_t n)
        {
            if(n>0)
            {
                if(0!=code)
                {
                    Code *newCode = new Code(n+code->maxBlocks);
                    Memory::OutOfReach::Grab(newCode->base,code->base,(newCode->size=code->size)*sizeof(T));
                    code->size = 0;
                    delete code;
                    code = newCode;
                }
                else
                {
                    code = new Code(n);
                }
            }
        }


    private:
        Y_DISABLE_ASSIGN(OrderedVector);
        class   Code;
        typedef Memory::Wad<MutableType,ALLOCATOR> WadType;

        inline void release_() noexcept { if(0!=code) { delete code; code=0; } }

        Code *code;

        class Code : public Object, public WadType
        {
        public:
            inline explicit Code(const size_t n) :
            Object(),
            WadType(n),
            base(static_cast<MutableType*>(this->workspace) ),
            data(base-1),
            size(0)
            {
                assert(n>0);
            }

            inline explicit Code(const Code &impl) :
            Object(),
            WadType(impl->size),
            base(static_cast<MutableType*>(this->workspace) ),
            data(base-1),
            size(0)
            {
                assert(impl->size>0);
                try {
                    while(size<impl->size)
                    {
                        new (base+size) MutableType( impl->base[size] );
                        ++size;
                    }
                }
                catch(...)
                {
                    free();
                    throw;
                }
            }



            inline virtual ~Code() noexcept { free(); }

            inline void free() noexcept { Orderly::Finish(base,size); }

            MutableType * const base;
            MutableType * const data;
            size_t              size;

        private:
            Y_DISABLE_ASSIGN(Code);
        };

        static inline Code * Duplicate(const Code *impl)
        {
            if(impl)
            {
                const size_t n = impl->size;
                if(n>0)
                {
                    return new Code(*impl);
                }
                else
                {
                    return 0;
                }
            }
            else
                return 0;
        }
    };

}


#endif
