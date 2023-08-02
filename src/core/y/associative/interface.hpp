//! \file

#ifndef Y_Associative_Included
#define Y_Associative_Included 1

#include "y/container/dynamic.hpp"
#include "y/type/args.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Associative interface
    //
    //
    //__________________________________________________________________________
    template <typename KEY, typename T>
    class Associative : public Dynamic
    {
    public:
        Y_ARGS_DECL(T,Type);
        Y_ARGS_DECL(KEY,Key);

    protected:
        inline explicit Associative() noexcept {}
    public:
        inline virtual ~Associative() noexcept {}

        virtual ConstType *search(const ParamKey key) const noexcept = 0;
       


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Associative);
    };

}

#endif

