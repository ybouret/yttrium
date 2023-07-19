//! \file

#ifndef Y_Memory_OutOfReach_Included
#define Y_Memory_OutOfReach_Included 1

#include "y/config/starting.hpp"
#include "y/type/destruct.hpp"

namespace Yttrium
{
    namespace Memory
    {
        //______________________________________________________________________
        //
        //
        //
        //! algorithms on out-of-reach memory
        //
        //
        //______________________________________________________________________
        struct OutOfReach
        {
            static void *       Addr(void *       addr)                                           noexcept; //!< addr
            static const void * Addr(const void * addr)                                           noexcept; //!< addr
            static void *       Zero(void *       addr,   const size_t size)                      noexcept; //!< memset(addr,0,size)
            static void *       Copy(void *       target, const void * source, const size_t size) noexcept; //!< memcpy(target,source,size)
            static void *       Grab(void *       target, void       * source, const size_t size) noexcept; //!< memcpy/zero
            static void *       Move(void *       target, const void * source, const size_t size) noexcept; //!< memove(target,source,size)
            static void *       Swap(void *       target, void *       source, const size_t size) noexcept; //!< memswap(target,source,size
            static ptrdiff_t    Diff(const void * a,      const void * b)                         noexcept; //!< b-a in bytes
            static bool         Are0(const void * addr,   const size_t size)                      noexcept; //!< check all bytes are 0
            static void *       Haul(void *addr, ptrdiff_t delta)                                 noexcept; //!< return addr+delta
            static void         Lift(void *addr, size_t size, const size_t blockSize)             noexcept; //!< addr+size -> addr+size+blockSize, addr[blockSize] = 0
            static void         Fill(void *tgt, const size_t, const void *src, const size_t)      noexcept; //!< fill tgt with copy(ies) of src
            static void         Move(void *a, void *b, const void *c, const size_t size)          noexcept; //!< a=b,b=c

            //! zeroed, destructed item
            template <typename T> static inline
            T *Naught(T *item) noexcept
            {
                return static_cast<T*>(Zero(Destructed(item),sizeof(T)));
            }

            //! force Cast
            template <typename T, typename U> static inline
            T *Cast(U *source) noexcept
            {
                return static_cast<T*>( Addr(source) );
            }

            //! force Haul
            template <typename T> static inline
            T *Shift(T *item, const ptrdiff_t n) noexcept
            {
                static const ptrdiff_t sz = sizeof(T);
                return static_cast<T*>( Haul(item,n*sz) );
            }

            template <typename T> static inline
            void Swap(T &a, T &b) noexcept
            {
                Swap(&a,&b,sizeof(T));
            }

        };
    }

}

#define Y_STATIC_ZVAR(VAR) Yttrium::Memory::OutOfReach::Zero(&VAR,sizeof(VAR)) //!< cleanup a variable
#define Y_STATIC_ZARR(ARR) Yttrium::Memory::OutOfReach::Zero(ARR, sizeof(ARR)) //!< cleanup an array

#endif

