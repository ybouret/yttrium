//! \file

#ifndef Y_Sequence_List_Included
#define Y_Sequence_List_Included 1

#include "y/sequence/interface.hpp"
#include "y/container/dynamic.hpp"
#include "y/container/writable.hpp"
#include "y/type/capacity.hpp"
#include "y/data/small/heavy/solo-list.hpp"

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
        class List
        {
        public: static const char * const CallSign; //!< "Vector"
        protected: explicit List() noexcept;        //!< setup
        public:    virtual ~List() noexcept;        //!< cleanup
        private:   Y_DISABLE_COPY_AND_ASSIGN(List);
        };
    }


    //__________________________________________________________________________
    //
    //
    //
    //! List of data
    //
    //
    //__________________________________________________________________________
    template <typename T>
    class List :
    public Dynamic,
    public Sequence<T>,
    public Core::List,
    public Writable<T>
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_DECL(T,Type); //!< aliases
        typedef Small::SoloHeavyList<T> Code;

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        inline explicit List() noexcept:  code() {}   //!< setup empty
        inline virtual ~List() noexcept {  } //!< cleanup
        inline explicit List(const size_t n, const AsCapacity_ &) : code() { code.proxy->reserve(n); }
        inline List(const List &other) : code(other.code) {}
        inline List & operator=(const List &other)
        {
            code = other.code;
            return *this;
        }

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline virtual const char * callSign() const noexcept { return CallSign; }
        inline virtual size_t size()     const noexcept { return code.size; }
        inline virtual size_t capacity() const noexcept { return code.size + code.proxy->stowage(); }
        inline virtual size_t available() const noexcept { return code.proxy->stowage(); }
        inline virtual void   reserve(const size_t n)  { code.proxy->reserve(n); }
        inline virtual void   free() noexcept { code.release(); }
        inline virtual void   release() noexcept { code.release(); code.proxy->release(); }

        inline virtual void pushTail(ParamType args) { code << args; }
        inline virtual void pushHead(ParamType args) { code >> args; }

        inline virtual void popHead() noexcept { code.cutHead(); }
        inline virtual void popTail() noexcept { code.cutTail(); }

        inline Type & operator[](const size_t i) noexcept
        {
            return **code.fetch(i);
        }

        inline const Type & operator[](const size_t i) const noexcept
        {
            return **code.fetch(i);
        }



    private:
        Code code;
    };

}

#endif
