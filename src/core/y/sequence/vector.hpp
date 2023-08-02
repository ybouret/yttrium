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
#include "y/type/capacity.hpp"
#include "y/container/iterator/writable-contiguous.hpp"

namespace Yttrium
{

    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //! Base class to hold definitions
        //
        //______________________________________________________________________
        class Vector
        {
        public: static const char * const CallSign;  //!< "Vector"
        protected: explicit Vector() noexcept;       //!< setup
        public:    virtual ~Vector() noexcept;       //!< cleanup
        private:   Y_DISABLE_COPY_AND_ASSIGN(Vector);
        };
    }

    //______________________________________________________________________
    //
    //! helper for constructor
    //______________________________________________________________________
#define Y_Vector_Prolog() \
Identifiable(), Collection(), Dynamic(), Sequence<T>(), Core::Vector(), Writable<T>(), WritableContiguous<T>()

    //__________________________________________________________________________
    //
    //
    //
    //! Vector of data
    //
    //
    //__________________________________________________________________________
    template <typename T, typename ALLOCATOR = Memory::Global>
    class Vector :
    public Dynamic,
    public Sequence<T>,
    public Core::Vector,
    public Writable<T>,
    public WritableContiguous<T>
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_DECL(T,Type);                               //!< aliases
        typedef Memory::Workspace<MutableType> Workspace;  //!< alias
        typedef Memory::OutOfReach             MemOps;     //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        inline   Vector() noexcept: Y_Vector_Prolog(), code( 0 ) {}   //!< setup empty
        virtual ~Vector() noexcept { release_(); }                    //!< cleanup

        //! setup with a given capacity
        inline Vector(const size_t n, const AsCapacity_&) : Y_Vector_Prolog(), code( new Code(n) )
        { assert( capacity() >= n ); }

        //! copy
        inline Vector(const Vector &other) : Y_Vector_Prolog(), code( Duplicate(other) ) {}

        //! no-throw exchange
        inline void swapWith(Vector &other) noexcept { Swap(code,other.code); }

        //! assign
        inline Vector & operator=(const Vector &other)
        { Vector tmp(other); swapWith(tmp); return *this; }


        //______________________________________________________________________
        //
        //
        // specific constructor
        //
        //______________________________________________________________________

        //! setup with the same value
        inline Vector(const size_t n, ParamType args) :
        Y_Vector_Prolog(), code( (n>0) ? new Code(n,args) : 0) { assert(size()==n); }


        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline virtual const char * callSign() const noexcept { return CallSign; }
        inline virtual size_t       size()     const noexcept { return code ? code->size     :  0; }
        inline virtual size_t       capacity() const noexcept { return code ? code->maxBlocks : 0; }
        inline virtual void         free()           noexcept { if(0!=code) code->free(); }
        inline virtual void         release()        noexcept { release_(); }
        inline virtual void         reserve(const size_t n)
        {
            if(n<=0) return;
            minimalCapacity(capacity()+n);
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

        //______________________________________________________________________
        //
        //
        // iterators
        //
        //______________________________________________________________________


    private:
        class Code;
        Code *code;

        inline virtual ConstType & getHead() const noexcept
        {
            assert(code!=0);
            assert(code->size>0);
            assert(0!=code->base);
            return *(code->base);
        }

        inline virtual ConstType & getTail() const noexcept
        {
            assert(code!=0);
            assert(code->size>0);
            assert(0!=code->item);
            return code->item[code->size];
        }



        inline virtual ConstType * getBaseForward() const noexcept
        {
            return 0!=code ? code->base : 0;
        }

        inline virtual ConstType * getLastForward() const noexcept
        {
            return 0!=code ? code->base+code->size : 0;
        }

        inline virtual ConstType * getBaseReverse() const noexcept
        {
            return 0!=code ? code->item+code->size : 0;
        }

        inline virtual ConstType * getLastReverse() const noexcept
        {
            return 0!=code ? code->item : 0;
        }



        //! self releasing
        inline void release_() noexcept { if(0!=code) { delete code; code=0; } }

        //! ensure minical capacity
        inline void minimalCapacity(const size_t n)
        {
            if(0==code)
                code = new Code( n );
            else
            {
                assert(n>code->maxBlocks);
                Code *temp = new Code(n);
                Memory::OutOfReach::Grab(temp->base,code->base,(temp->size=code->size)*sizeof(T));
                code->size = 0;
                delete code;
                code = temp;
            }
            assert(0!=code);
            assert(code->maxBlocks>=n);
        }

        //! check enough space to insert one item
        inline void checkSpace()
        {
            const size_t capa = capacity();
            if(size()>=capa)
                minimalCapacity( this->NextCapacity(capa) );
            assert(0!=code);
            assert(code->size<code->maxBlocks);
        }

        static inline Code *Duplicate(const Vector &other)
        {
            const size_t n = other.size();
            if(n>0)
            {
                assert(0!=other.code);
                Code        *myCode = new Code(n);
                ConstType   *source = other.code->base;
                MutableType *target = myCode->base;
                size_t      &i      = myCode->size;
                try {
                    while(i<n)
                    {
                        new (target+i) MutableType(source[i]);
                        ++i;
                    }
                    return myCode;
                }
                catch(...)
                {
                    delete myCode;
                    throw;
                }
            }
            else
            {
                return 0;
            }
        }

        //! setup vector code for at least NN objects
#define Y_Vector_Code_Prolog(NN)                  \
Object(), WadType(n),                             \
base(static_cast<MutableType*>(this->workspace)), \
item(base-1), size(0)

        //______________________________________________________________________
        //
        //
        //! Internal Code
        //
        //______________________________________________________________________
        class Code : public Object, public Memory::Wad<Type,ALLOCATOR>
        {
        public:
            typedef Memory::Wad<Type,ALLOCATOR> WadType; //!< alias

            //__________________________________________________________________
            //
            //! get formated flat memory
            //__________________________________________________________________
            inline explicit Code(const size_t n) : Y_Vector_Code_Prolog(n)
            {
                assert(this->maxBlocks>=n);
            }


            //__________________________________________________________________
            //
            //! cleanup before destruction
            //__________________________________________________________________
            inline virtual ~Code() noexcept { free(); }

            //__________________________________________________________________
            //
            //! get formated memory with common args
            //__________________________________________________________________
            inline explicit Code(const size_t n, ConstType &args) : Y_Vector_Code_Prolog(n)
            {
                assert(this->maxBlocks>=n);
                try {
                    while(size<n) { new (base+size) MutableType(args); ++size; }
                }
                catch(...) { free(); throw; }
            }

            //__________________________________________________________________
            //
            //! free content
            //__________________________________________________________________
            inline void     free() noexcept { while(size>0) popTail(); }

            //__________________________________________________________________
            //
            //! pop tail
            //__________________________________________________________________
            inline void     popTail() noexcept { assert(size>0); MemOps::Naught( &base[--size] ); }

            //__________________________________________________________________
            //
            //! pop head
            //__________________________________________________________________
            inline void     popHead() noexcept { assert(size>0);
                MemOps::Move( MemOps::Naught(base),base+1,(--size)*sizeof(T));
                MemOps::Zero(base+size,sizeof(T));
            }

            //__________________________________________________________________
            //
            //! insert at tail with enough memory
            //__________________________________________________________________
            inline void pushTail(ConstType &temp) noexcept
            {
                assert(size<this->maxBlocks);
                MemOps::Copy(&base[size++],&temp,sizeof(T));
            }

            //__________________________________________________________________
            //
            //! insert at head with enough memory
            //__________________________________________________________________
            inline void pushHead(ConstType &temp) noexcept
            {
                assert(size<this->maxBlocks);
                MemOps::Move(base+1,base,size*sizeof(T));
                MemOps::Copy(base,&temp,sizeof(T));
                ++size;
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            MutableType * const base; //! [0..size-1]
            MutableType * const item; //!< [1..size]
            size_t              size; //!< 0..maxBlocks

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };
    };
}

#endif
