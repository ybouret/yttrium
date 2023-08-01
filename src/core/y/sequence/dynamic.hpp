
//! \file

#ifndef Y_Dynamic_Sequence_Included
#define Y_Dynamic_Sequence_Included 1

#include "y/container/dynamic.hpp"
#include "y/sequence/interface.hpp"

namespace Yttrium
{

    template <typename T>
    class DynamicSequence : public Dynamic, public Sequence<T>
    {
    protected:
        inline explicit DynamicSequence() noexcept : Dynamic(), Sequence<T>() {}
    public:
        inline virtual ~DynamicSequence() noexcept {}


        inline void trim(size_t n) noexcept
        {
            while(n-- > 0 && this->size() > 0) this->popTail();
        }

        inline void skip(size_t n) noexcept
        {
            while(n-- > 0 && this->size() > 0) this->popHead();
        }
        


    private:
        Y_DISABLE_COPY_AND_ASSIGN(DynamicSequence);
    };

}

#endif
