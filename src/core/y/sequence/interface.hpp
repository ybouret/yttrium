//! \file

#ifndef Y_Sequence_Interface_Included
#define Y_Sequence_Interface_Included 1

#include "y/type/args.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Sequence interface
    //
    //
    //__________________________________________________________________________
    template <typename T>
    class Sequence
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_DECL(T,Type);

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
    protected:
        inline explicit Sequence() noexcept {} //!< setup

    public:
        inline virtual ~Sequence() noexcept {} //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        virtual void pushHead(ParamType) = 0;
        virtual void pushTail(ParamType) = 0;
        virtual void popTail() noexcept = 0; //!< remove tail object
        virtual void popHead() noexcept = 0; //!< remove head object

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Sequence);
    };

}

#endif

