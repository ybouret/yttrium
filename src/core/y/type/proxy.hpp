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
        inline ConstInterface * operator->() const noexcept { return & surrogate(); } //!< access
        inline ConstInterface & operator*()  const noexcept { return   surrogate(); } //!< access

        inline Interface * operator->() noexcept
        {
            ConstInterface & _ = surrogate();
            return (Interface *) &_;
        } //!< access

        inline Interface & operator*() noexcept {
            ConstInterface & _ = surrogate();
            return (Interface &)_;
        } //!< access


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


    //______________________________________________________________________
    //
    //
    //! helper for private surrogate declaration
    //
    //______________________________________________________________________
#define Y_PROXY_DECL() virtual ConstInterface & surrogate() const noexcept

    //______________________________________________________________________
    //
    //
    //! helper for private surrogate implementation
    //
    //______________________________________________________________________
#define Y_PROXY_IMPL(CLASS,MY) \
CLASS::ConstInterface & CLASS:: surrogate() const noexcept { return MY; }

}

#endif

