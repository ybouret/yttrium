
//! \file

#ifndef Y_Container_Readable_Included
#define Y_Container_Readable_Included 1

#include "y/container/interface.hpp"
#include "y/type/args.hpp"

namespace Yttrium
{
    template <typename T> class Readable : public Container
    {
    public:
        Y_ARGS_EXPOSE(T);

    protected:
        inline explicit Readable() noexcept : Container() {}

    public:
        inline virtual ~Readable() noexcept {   }

        virtual ConstType & operator[](const size_t) const noexcept = 0;



    private:
        Y_DISABLE_COPY_AND_ASSIGN(Readable);
    };
}

#endif
