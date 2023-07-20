

//! \file

#ifndef Y_Container_Writable_Included
#define Y_Container_Writable_Included 1

#include "y/container/readable.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Writable container
    //
    //
    //__________________________________________________________________________
    template <typename T> class Writable : public Readable<T>
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_EXPOSE(T); //!< aliases

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
    protected:
        inline explicit Writable() noexcept : Readable<T>() {} //!< setup
    public:
        inline virtual ~Writable() noexcept { }                //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        virtual Type & operator[](const size_t) noexcept = 0; //!< access in [1..size()]



    private:
        Y_DISABLE_COPY_AND_ASSIGN(Writable);
    };
}

#endif
