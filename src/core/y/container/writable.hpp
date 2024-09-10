

//! \file

#ifndef Y_Container_Writable_Included
#define Y_Container_Writable_Included 1

#include "y/container/readable.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Writable container
    //
    //
    //__________________________________________________________________________
    template <typename T> class Writable : public Readable<T>
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
        inline explicit Writable() noexcept : Readable<T>() {} //!< setup
    public:
        inline virtual ~Writable() noexcept { }                //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        virtual ConstType & operator[](const size_t) const noexcept = 0; //!< access in [1..size()], avoids overload warning
        virtual Type &      operator[](const size_t) noexcept       = 0; //!< access in [1..size()]

        //! load same value(s)
        inline Writable & ld(ParamType sameValue)
        {
            Writable<T> &self = *this;
            for(size_t i=self.size();i>0;--i) self[i] = sameValue;
            return self;
        }

        //! load a copy
        template <typename U> inline
        Writable & ld(const Readable<U> &other)
        {
            Writable<T> &self = *this;
            assert(other.size()>=self.size());
            for(size_t i=self.size();i>0;--i) self[i] = other[i];
            return self;
        }


        //! apply the same method to all data
        template <typename METHOD_POINTER> inline
        Writable &forEach(METHOD_POINTER meth)
        {
            Writable<T> &self = *this;
            for(size_t i=self.size();i>0;--i)
            {
                (self[i].*(meth))();
            }
            return self;
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Writable);
    };
}

#endif
