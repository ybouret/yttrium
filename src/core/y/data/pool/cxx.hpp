//! \file

#ifndef Y_Data_Pool_Cxx_Included
#define Y_Data_Pool_Cxx_Included 1

#include "y/data/pool.hpp"
#include "y/type/releasable.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! C++ type pool, assuming NODE is a C++ (copiable) class
    //
    //
    //__________________________________________________________________________
    template <typename NODE>
    class CxxPoolOf : public PoolOf<NODE>, public Releasable
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef PoolOf<NODE> PoolType;   //!< alias
        using PoolType::size;
        using PoolType::query;
        using PoolType::store;
        using PoolType::reverse;

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit CxxPoolOf() noexcept : PoolType(), Releasable() { } //!< setup
        virtual ~CxxPoolOf() noexcept { release(); }                 //!< cleanup

        //! copy using NODE copy constructor
        CxxPoolOf( const CxxPoolOf &other ) : PoolType(), Releasable()
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
        Y_DISABLE_ASSIGN(CxxPoolOf);
        inline void release_() noexcept { while(size>0) delete query(); }

        inline void duplicate_(const PoolType &other)  
        {
            try
            {
                for(const NODE *node=other.head;node;node=node->next)
                    store( new NODE(*node) );
                reverse();
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

