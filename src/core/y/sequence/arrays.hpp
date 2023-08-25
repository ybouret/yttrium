//! \file

#ifndef Y_Sequence_Arrays_Included
#define Y_Sequence_Arrays_Included 1

#include "y/container/light-array.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/memory/wad.hpp"
#include "y/container/operating.hpp"
#include "y/object.hpp"

namespace Yttrium
{

    namespace Core
    {
        class Arrays
        {
        public:
            static const char * const CallSign;
            virtual ~Arrays() noexcept;

        protected:
            explicit Arrays() noexcept;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Arrays);
        };
    }

    template <typename T, typename ALLOCATOR>
    class Arrays :
    public Memory::Wad<LightArray<T>,Memory::Pooled>,
    public Writable< LightArray<T> >
    {
    public:

    private:
        
    };

#if 0
    template <typename T, typename ALLOCATOR>
    class Arrays :
    public Memory::Wad<LightArray<T>,Memory::Pooled>,
    public Writable< LightArray<T> >
    {
    public:
        Y_ARGS_EXPOSE(T,Type);
        static const char * const CallSign;

        typedef LightArray<T>                         ArrayType;
        typedef Memory::Wad<ArrayType,Memory::Pooled> ArrayWad;
        typedef Memory::Wad<MutableType,ALLOCATOR>    DataWad;
        typedef Operating<MutableType>                DataMgr;

        inline explicit Arrays(const size_t numArrays,
                               const size_t iniBlocks) :
        ArrayWad(numArrays),
        arr( static_cast<ArrayType*>(this->workspace) ),
        sub(arr-1),
        num( numArrays ),
        len( iniBlocks ),
        code( new Code(num*len)     )
        {
            link();
        }

        inline virtual ~Arrays() noexcept
        {
            assert(0!=code);
            cleanup();
            delete code;
            code = 0;
        }

        inline virtual const char * callSign() const noexcept { return "Arrays"; }

        inline virtual  size_t size() const noexcept { return num; }

        inline virtual ArrayType & operator[](const size_t indx) const noexcept
        { return sub[indx]; }

        inline virtual ArrayType & operator[](const size_t indx)  noexcept
        { return sub[indx]; }

        inline void make(const size_t n)
        {
            if(n!=len)
            {
                Code *newCode = new Code(num*n);
                cleanup();
                delete code;
                code = newCode;
                link();
            }
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Arrays);
        class Code;

        ArrayType * const arr;
        ArrayType * const sub;
        const size_t      num;
        size_t            len;
        Code             *code;

        inline void cleanup() noexcept {
            for(size_t i=num;i>0;--i) Destruct( &sub[i] );
        }

        void link() noexcept
        {
            assert(Good(arr,num));
            assert(0!=code);
            MutableType *entry = code->data;
            for(size_t i=0;i<num;++i, entry += len)
            {
                new ( &arr[i] ) ArrayType(entry,len);
            }
        }

        class Code : public Object, public DataWad, public DataMgr
        {
        public:
            explicit Code(const size_t n) :
            DataWad(n),
            DataMgr(this->workspace,n),
            data( static_cast<MutableType *>(this->workspace) )
            {
            }

            virtual ~Code() noexcept {}

            MutableType * const data;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

    };
#endif

}

#endif

