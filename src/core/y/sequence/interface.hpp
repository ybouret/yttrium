//! \file

#ifndef Y_Sequence_Interface_Included
#define Y_Sequence_Interface_Included 1

#include "y/container/collection.hpp"
#include "y/type/args.hpp"


namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Sequence interface
    //
    //
    //__________________________________________________________________________
    template <typename T>
    class Sequence : public virtual Collection
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
    protected:
        inline explicit Sequence() noexcept {} //!< setup

    public:
        inline virtual ~Sequence() noexcept {} //!< cleanup

        //______________________________________________________________________
        //
        //
        // Interface
        //
        //______________________________________________________________________
        virtual void pushHead(ParamType) = 0; //!< push object at head
        virtual void pushTail(ParamType) = 0; //!< push object at tail
        virtual void popTail()  noexcept = 0; //!< remove tail object
        virtual void popHead()  noexcept = 0; //!< remove head object
        
        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline ConstType &head() const noexcept { return getHead(); }           //!< head object, const
        inline ConstType &tail() const noexcept { return getTail(); }           //!< tail object, const
        inline Type      &head()       noexcept { return Coerce(getHead()); }   //!< head object
        inline Type      &tail()       noexcept { return Coerce(getTail()); }   //!< tail object

        Sequence & operator<<(ParamType args) { pushTail(args); return *this; } //!< syntactic helper
        Sequence & operator>>(ParamType args) { pushHead(args); return *this; } //!< syntactic helper

        inline Type pullHead() { ConstType tmp = head(); popHead(); return tmp; } //!< get copy and remove head
        inline Type pullTail() { ConstType tmp = tail(); popTail(); return tmp; } //!< get copy and remove tail

        
        //! trim tail items
        inline void trim(size_t n) noexcept
        {
            while(n-- > 0 && this->size() > 0) this->popTail();
        }

        //! skip head items
        inline void skip(size_t n) noexcept
        {
            while(n-- > 0 && this->size() > 0) this->popHead();
        }

        //! adjust size with padding if necessary
        inline void adjust(const size_t n, ParamType pad) {
            while(this->size()>n) this->popTail();
            while(this->size()<n) this->pushTail(pad);
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Sequence);
        virtual ConstType &getHead() const noexcept = 0;
        virtual ConstType &getTail() const noexcept = 0;

    };

}

#endif

