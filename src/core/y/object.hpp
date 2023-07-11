//!\ file

#ifndef Y_Object_Included
#define Y_Object_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! base class for (small) objects
    //
    //
    //__________________________________________________________________________
    class Object
    {
    public:
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        virtual ~Object() noexcept;
        explicit Object() noexcept;

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
        static void * operator new    [](const size_t block_size);                           //!< new[block_size]
        static void   operator delete [](void *block_addr, const size_t block_size) noexcept; //!< delete[block_size]

        //______________________________________________________________________
        //
        // placement new()/delete()
        //______________________________________________________________________
        static void *operator new(size_t block_size, void *addr) noexcept; //!< placement new
        static void  operator delete(void *, void *) noexcept;             //!< placement delete

        //______________________________________________________________________
        //
        // helpers
        //______________________________________________________________________

        //! acquire one zombie block
        template <typename T> static inline
        T *zacquire() { return static_cast<T *>( Object::operator new(sizeof(T)) ); }

        //! release one zombie block
        template <typename T> static inline
        void zrelease(T *obj) noexcept { assert(obj); Object::operator delete(obj,sizeof(T)); }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Object);
    };

}

#endif

