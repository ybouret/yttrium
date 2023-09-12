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
#include "y/memory/solitary/workspace.hpp"
#include "y/container/iterator/readable-contiguous.hpp"

namespace Yttrium
{
    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //! common class for ordered vectors
        //
        //______________________________________________________________________
        class OrderedVector
        {
        public:
            static const char * const CallSign; //!< "OrderedVector"
            explicit OrderedVector() noexcept;  //!< setup
            virtual ~OrderedVector() noexcept;  //!< cleanup

            void multipleValueException() const; //!< raise exception
            void valueNotFoundException() const; //!< raise exception

        private:
            Y_DISABLE_COPY_AND_ASSIGN(OrderedVector);
        };
    }

    //__________________________________________________________________________
    //
    //
    //
    //! Ordered Vectors
    //
    //
    //__________________________________________________________________________
    template <
    typename T,
    typename COMPARATOR = IncreasingComparator,
    typename ALLOCATOR  = Memory::Pooled>
    class OrderedVector :
    public Dynamic,
    public Readable<T>,
    public ReadableContiguous<T>,
    public Core::OrderedVector
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_DECL(T,Type); //!< aliases

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! setup empty
        explicit OrderedVector() noexcept      : code(0) {}

        //! setup with capacity
        explicit OrderedVector(const size_t n) : code( n>0 ? new Code(n) :0 ) {}

        //! cleanup
        virtual ~OrderedVector() noexcept { release_(); }

        //! copy
        inline   OrderedVector(const OrderedVector &_) :
        Identifiable(), Collection(),
        Dynamic(),
        Readable<T>(),
        ReadableContiguous<T>(),
        Core::OrderedVector(),
        code( Duplicate(_.code) ) {}

        //! assign by copy/swap
        inline OrderedVector & operator=(const OrderedVector &other)
        {
            OrderedVector tmp(other);
            xch(tmp);
            return *this;
        }

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline virtual const char * callSign() const noexcept { return CallSign; }                     //!< CallSign
        inline virtual void         release()        noexcept { release_();      }                     //!< release all memory
        inline virtual size_t       size()     const noexcept { return 0!=code ? code->size     : 0; } //!< current size
        inline virtual size_t       capacity() const noexcept { return 0!=code ? code->capacity : 0; } //!< current capacity
        inline virtual void         free()           noexcept { if(0!=code) code->free();        }     //!< free content, keep memory

        //! [Readable] access
        inline virtual ConstType & operator[](const size_t indx) const noexcept
        {
            assert(0!=code);
            assert(indx>=1);
            assert(indx<=size());
            return code->data[indx];
        }

        //! reserve extra space
        inline virtual void reserve(size_t n)
        {
            if(n>0)
            {
                if(0!=code)
                {
                    Code *newCode = new Code(n+code->capacity);
                    Memory::OutOfReach::Grab(newCode->head,code->head,(newCode->size=code->size)*sizeof(T));
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

        //______________________________________________________________________
        //
        //
        // interface
        //
        //______________________________________________________________________

        //______________________________________________________________________
        //
        //! search for args
        //______________________________________________________________________
        inline bool search(ParamType args) const
        {
            size_t ipos = 0;
            return (0!=code) ? code->search(ipos,args) : false;
        }

        //______________________________________________________________________
        //
        //! insert args
        //______________________________________________________________________
        inline bool insert(ParamType args)
        {
            if(0==code)
            {
                //--------------------------------------------------------------
                // will be the first inserted
                //--------------------------------------------------------------
                code = new Code( NextCapacity(0) );
                const bool inserted = code->insert(args);
                assert(true==inserted);
                return inserted;
            }
            else
            {
                if(code->size<code->capacity)
                {
                    //----------------------------------------------------------
                    // regular case, code do all the work
                    //----------------------------------------------------------
                    return code->insert(args);
                }
                else
                {
                    //----------------------------------------------------------
                    // need to increase capacity while preserving args
                    //----------------------------------------------------------
                    Memory::Workspace<MutableType> temp;
                    ConstType &alias = temp.build(args);
                    reserve( NextIncrease(code->capacity) );
                    return code->insert(alias);
                }
            }
        }

        //______________________________________________________________________
        //
        //! remove args
        //______________________________________________________________________
        inline bool remove(ParamType args)
        {
            return (0!=code) ? code->remove(args) : false;
        }


        //______________________________________________________________________
        //
        //! no throw exchange
        //______________________________________________________________________
        inline void xch(OrderedVector &other) noexcept
        {
            Swap(code,other.code);
        }

        //______________________________________________________________________
        //
        //! add a NON-existing args
        //______________________________________________________________________
        inline OrderedVector & operator+=(ParamType args) {
            if(!insert(args)) multipleValueException();
            return *this;
        }

        //______________________________________________________________________
        //
        //! add any args
        //______________________________________________________________________
        inline OrderedVector & operator |=(ParamType args)
        {
            (void) insert(args);
            return *this;
        }

        //______________________________________________________________________
        //
        //! remove a pre-existing arg
        //______________________________________________________________________
        inline OrderedVector & operator -=(ParamType args)
        {
            if(!remove(args)) valueNotFoundException();
            return *this;
        }

        //______________________________________________________________________
        //
        //! remove any arg
        //______________________________________________________________________
        inline OrderedVector & operator ^=(ParamType args)
        {
            (void)remove(args);
            return *this;
        }

        //______________________________________________________________________
        //
        //! merge values
        //______________________________________________________________________
        inline OrderedVector & operator |= (const OrderedVector &rhs)
        {
            const Code *source = rhs.code;
            if(0!=source)
            {
                for(size_t i=source->size;i>0;--i)
                    (void) insert(source->data[i]);
            }
            return *this;
        }

        //______________________________________________________________________
        //
        //! exclude values
        //______________________________________________________________________
        inline OrderedVector & operator ^= (const OrderedVector &rhs)
        {
            const Code *source = rhs.code;
            if(0!=source)
            {
                for(size_t i=source->size;i>0;--i)
                    (void) remove(source->data[i]);
            }
            return *this;
        }


        
    private:
        class   Code;
        typedef Memory::Wad<MutableType,ALLOCATOR> WadType;

        inline void               release_()             noexcept { if(0!=code) { delete code; code=0; } }
        inline virtual ConstType *getBaseForward() const noexcept { return code?code->head:0; }
        inline virtual ConstType *getLastForward() const noexcept { return code?code->head+code->size:0; }

        inline virtual ConstType *getBaseReverse() const noexcept { return code?code->data+code->size:0; }
        inline virtual ConstType *getLastReverse() const noexcept { return code?code->data:0;            }

        Code *code;

        class Code : public Object, public WadType
        {
        public:
            inline explicit Code(const size_t n) :
            Object(),
            WadType(n),
            head(static_cast<MutableType*>(this->workspace) ),
            data(head-1),
            size(0),
            proc()
            {
                assert(n>0);
            }

            inline explicit Code(const Code &impl) :
            Object(),
            WadType(impl.size),
            head(static_cast<MutableType*>(this->workspace) ),
            data(head-1),
            size(0),
            proc()
            {
                assert(impl.size>0);
                try {
                    while(size<impl.size)
                    {
                        new (head+size) MutableType( impl.head[size] );
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
            inline void     free() noexcept { Orderly::Finish(head,size); }

            inline bool search(size_t    &ipos,
                               ConstType &args) const
            {
                return Orderly::Locate(ipos,args,static_cast<ConstType*>(head),size,proc);
            }

            inline bool insert(ConstType &args)
            {
                assert(size<this->capacity);
                size_t ipos = 0;
                if(search(ipos,args))
                    return false; // already exists
                else
                {
                    Orderly::Insert(ipos,args,head,size);
                    return true;
                }
            }

            inline bool remove(ConstType &args)
            {
                size_t ipos = 0;
                if(search(ipos,args))
                {
                    Orderly::Remove(ipos,head,size);
                    return true;
                }
                else
                    return false;
            }


            MutableType * const head;
            MutableType * const data;
            size_t              size;
            COMPARATOR          proc;

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
