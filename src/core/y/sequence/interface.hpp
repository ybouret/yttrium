//! \file

#ifndef Y_Sequence_Interface_Included
#define Y_Sequence_Interface_Included 1

#include "y/type/args.hpp"

namespace Yttrium
{

    template <typename T>
    class Sequence
    {
    public:


    protected:
        inline explicit Sequence() noexcept {}

    public:
        inline virtual ~Sequence()
        virtual void popTail() noexcept = 0;;
        virtual void popHead() noexcept = 0;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Sequence);
    };

}

#endif

