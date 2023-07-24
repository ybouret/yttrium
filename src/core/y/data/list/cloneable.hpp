
//! \file

#ifndef Y_Data_List_Cloneable_Included
#define Y_Data_List_Cloneable_Included 1

#include "y/data/list.hpp"
#include "y/type/releasable.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! C++ type list, assuming NODE is a C++ CLONEABLE class
    //
    //
    //__________________________________________________________________________
    template <typename NODE>
    class  ListOfCloneable : public ListOf<NODE>, public Releasable
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
        explicit ListOfCloneable() noexcept : ListType(), Releasable() { } //!< setup
        virtual ~ListOfCloneable() noexcept { release(); }                 //!< cleanup

        //! copy using NODE copy constructor
        ListOfCloneable( const ListOfCloneable &other ) : ListType(), Releasable()
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
        Y_DISABLE_ASSIGN(ListOfCloneable);
        inline void release_() noexcept { while(size>0) delete popTail(); }
        inline void duplicate_(const ListType &other)
        {
            try
            {
                for(const NODE *node=other.head;node;node=node->next)
                    pushTail( node->clone() );
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
