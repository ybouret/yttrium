//! \file

#ifndef Y_Type_Proxy_Included
#define Y_Type_Proxy_Included 1

#include "y/type/args.hpp"

namespace Yttrium
{

    //______________________________________________________________________
    //
    //
    //
    //! Proxy pattern
    //
    //
    //______________________________________________________________________
    template <typename T>
    class Proxy
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_EXPOSE(T,Type); //!< aliases

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline Type *      operator->()       noexcept { return & surrogate(); } //!< access
        inline ConstType * operator->() const noexcept { return & surrogate(); } //!< access

        
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        inline virtual ~Proxy() noexcept {} //!< cleanup
    protected:
        inline explicit Proxy() noexcept {} //!< setup

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Proxy);
        virtual ConstType & surrogate() const noexcept = 0;
    };


}

#endif

