
//! \file

#ifndef Y_Container_Extensible_Included
#define Y_Container_Extensible_Included 1

#include "y/type/releasable.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Extensible class
    //
    //
    //__________________________________________________________________________
    class Extensible : public Releasable
    {
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
    protected:
        explicit Extensible() noexcept; //!< setup

    public:
        virtual ~Extensible() noexcept; //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        virtual void free()       noexcept = 0; //!< keep possible memory
        virtual void reserve(const size_t) = 0; //!< make room for extra objects

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Extensible);
    };

}

#endif
