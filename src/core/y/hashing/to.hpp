//! \file

#ifndef Y_HASHING_TO_INCLUDED
#define Y_HASHING_TO_INCLUDED 1


#include "y/hashing/function.hpp"
#include "y/associative/key-variety.hpp"

namespace Yttrium
{

    namespace Hashing
    {
        //______________________________________________________________________
        //
        //! function content to integral type
        //______________________________________________________________________
        template <typename T> inline
        T To(Function &H) noexcept { T res(0); H.get(&res,sizeof(T)); return res; }

        //______________________________________________________________________
        //
        //! integral hash of a block
        //______________________________________________________________________
        template <typename T> inline
        T To(Function &H, const void *block_addr, const size_t block_size) noexcept
        { T res(0); H.block(&res,sizeof(T),block_addr,block_size); return res; }

        //______________________________________________________________________
        //
        //! integral hash of a msg
        //______________________________________________________________________
        template <typename T> inline
        T To(Function &H, const char *msg) noexcept
        { T res(0); H.block(&res,sizeof(T),msg); return res; }


        //______________________________________________________________________
        //
        //! integral hash of a buffer
        //______________________________________________________________________
        template <typename T> inline
        T To(Function &H, const Memory::ReadOnlyBuffer &buf) noexcept
        { T res(0); H.block(&res,sizeof(T),buf); return res; }


      


    }

}

#endif

