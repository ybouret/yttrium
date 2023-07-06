
//! \file

#ifndef Y_Data_List_Cxx_Included
#define Y_Data_List_Cxx_Included 1

#include "y/data/list.hpp"
#include "y/type/releasable.hpp"

namespace Yttrium
{

    template <typename NODE>
    class CxxListOf : public ListOf<NODE>, public Releasable
    {
    public:
        typedef ListOf<NODE> ListType;
        using ListType::size;
        using ListType::popTail;
        using ListType::pushTail;

        explicit CxxListOf() noexcept : ListType(), Releasable() {          }
        virtual ~CxxListOf() noexcept { release(); }
        CxxListOf( const CxxListOf &other ) : ListType(), Releasable()
        {
            duplicate_(other);
        }

        virtual void release() noexcept { release_(); }

    private:
        Y_DISABLE_ASSIGN(CxxListOf);
        inline void release_() noexcept { while(size>0) delete popTail(); }
        inline void duplicate_(const ListType &other) noexcept
        {
            try
            {
                for(const NODE *node=other.head;node;node=node->next)
                    pushBack( new NODE(*node) );
            }
            catch(...)
            {
                release_();
                throw;
            }
        }
    };

}

#endif
