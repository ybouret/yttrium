

//! \file

#ifndef Y_Container_Writable_Included
#define Y_Container_Writable_Included 1

#include "y/container/readable.hpp"

namespace Yttrium
{
    template <typename T> class Writable : public Readable<T>
    {
    public:
        Y_ARGS_EXPOSE(T);

    protected:
        inline explicit Writable() noexcept : Readable<T>() {}

    public:
        inline virtual ~Writable() noexcept { }

        virtual Type & operator[](const size_t) noexcept = 0;



    private:
        Y_DISABLE_COPY_AND_ASSIGN(Writable);
    };
}

#endif
