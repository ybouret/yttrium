
//! \file

#ifndef Y_Data_List_Cxx_Included
#define Y_Data_List_Cxx_Included 1

#include "y/data/list.hpp"

namespace Yttrium
{

    template <typename NODE>
    class CxxListOf : public ListOf<NODE>
    {
    public:
        typedef ListOf<NODE> ListType;
        using ListType::size;
        using ListType::popTail;

        explicit CxxListOf() noexcept : ListOf<NODE>()
        {

        }

        virtual ~CxxListOf() noexcept
        {
            Release();
        }

        virtual void Release() noexcept
        {
            while(size>0) delete popTail();
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(CxxListOf);
    };

}

#endif
