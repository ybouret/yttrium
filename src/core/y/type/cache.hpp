
//! \file

#ifndef Y_Type_Cache_Included
#define Y_Type_Cache_Included 1

#include "y/type/releasable.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Cache interface
    //
    //
    //__________________________________________________________________________
    class Cache : public Releasable
    {

    protected:
        explicit     Cache() noexcept;              //!< setup
    public:
        virtual     ~Cache() noexcept;              //!< cleanup
        virtual void gc(const size_t) noexcept = 0; //!< garbage collector

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Cache);
    };

}

#endif
