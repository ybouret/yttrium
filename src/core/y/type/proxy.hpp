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
    template <typename INTERFACE>
    class Proxy
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_EXPOSE(INTERFACE,Interface); //!< aliases

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline Interface *      operator->()       noexcept { return & surrogate(); } //!< access
        inline ConstInterface * operator->() const noexcept { return & surrogate(); } //!< access
        inline Interface &      operator*()        noexcept { return   surrogate(); } //!< access
        inline ConstInterface & operator*()  const noexcept { return   surrogate(); } //!< access

        //! forward display call
        inline friend std::ostream & operator<<(std::ostream &os, const Proxy &self) { return os << self.surrogate(); }

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
        virtual ConstInterface & surrogate() const noexcept = 0;
    };

#define Y_PROXY_DECL() virtual ConstInterface & surrogate() const noexcept
#define Y_PROXY_IMPL(CLASS,MY) \
CLASS::ConstInterface & CLASS:: surrogate() const noexcept { return MY; }

}

#endif

