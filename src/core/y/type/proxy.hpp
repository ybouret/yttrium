//! \file

#ifndef Y_Type_Proxy_Included
#define Y_Type_Proxy_Included 1

#include "y/type/args.hpp"

namespace Yttrium
{

    template <typename T>
    class Proxy
    {
    public:
        Y_ARGS_EXPOSE(T,Type);
        inline virtual ~Proxy() noexcept {}

        inline Type *      operator->()       noexcept { return & surrogate(); }
        inline ConstType * operator->() const noexcept { return & surrogate(); }


    protected:
        inline explicit Proxy() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Proxy);
        virtual ConstType & surrogate() const noexcept = 0;
    };


}

#endif

