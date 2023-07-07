
//! \file

#ifndef Y_Data_List_Cxx_Included
#define Y_Data_List_Cxx_Included 1

#include "y/data/list.hpp"
#include "y/type/releasable.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! C++ type list, assuming NODE is a C++ (copiable) class
    //
    //
    //__________________________________________________________________________
    template <typename NODE>
    class CxxListOf : public ListOf<NODE>, public Releasable
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef ListOf<NODE> ListType;   //!< alias
        using ListType::size;
        using ListType::popTail;
        using ListType::pushTail;

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit CxxListOf() noexcept : ListType(), Releasable() { } //!< setup
        virtual ~CxxListOf() noexcept { release(); }                 //!< cleanup

        //! copy using NODE copy constructor
        CxxListOf( const CxxListOf &other ) : ListType(), Releasable()
        {
            duplicate_(other);
        }

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        virtual void release() noexcept { release_(); } //!< delete all nodes


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
