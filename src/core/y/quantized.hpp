
//! \file

#ifndef Y_Quantized_Included
#define Y_Quantized_Included 1


#include "y/config/starting.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! base class for quantized objects
    //
    //
    //__________________________________________________________________________
    class Quantized
    {
    public:
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        virtual ~Quantized() noexcept;
        explicit Quantized() noexcept;

        //______________________________________________________________________
        //
        // single object new/delete
        //______________________________________________________________________
        static void * operator new(const size_t blockSize);                             //!< new(blockSize)
        static void   operator delete(void *blockAddr,const size_t blockSize) noexcept; //!< delete(blockSize)

        //______________________________________________________________________
        //
        // multiple objects new[]/delete[]
        //______________________________________________________________________
        static void * operator new    [](const size_t blockSize);                           //!< new[block_size]
        static void   operator delete [](void *blockAddr, const size_t blockSize) noexcept; //!< delete[block_size]

        //______________________________________________________________________
        //
        // placement new()/delete()
        //______________________________________________________________________
        static void *operator new(size_t blockSize, void *addr) noexcept; //!< placement new
        static void  operator delete(void *, void *) noexcept;            //!< placement delete



    private:
        Y_DISABLE_COPY_AND_ASSIGN(Quantized);
    };

}

#endif

