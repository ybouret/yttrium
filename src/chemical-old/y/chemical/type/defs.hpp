//! \file

#ifndef Y_Chemical_Types_Included
#define Y_Chemical_Types_Included 1

#include "y/mkl/api.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/mkl/antelope/mul.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/associative/address-book.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/container/matrix.hpp"
#include "y/container/cxx/array.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef double                      real_t;      //!< hardware real
        typedef XReal<real_t>               xreal_t;     //!< software real
        typedef MKL::Antelope::Add<xreal_t> XAdd;        //!< alias
        typedef MKL::Antelope::Mul<xreal_t> XMul;        //!< alias
        typedef Memory::Dyadic              MemoryModel; //!< alias
        typedef Vector<xreal_t,MemoryModel> XVectorType; //!< vector of xreals
        typedef Matrix<xreal_t,MemoryModel> XMatrixType; //!< matrix of xreals
        typedef Readable<xreal_t>           XReadable;   //!< alias
        typedef Writable<xreal_t>           XWritable;   //!< alias
        typedef CxxArray<XAdd,MemoryModel>  XAddArray;   //!< alias

        //! helper to load a Small List from addresses
        template <typename TARGET>
        void SendBookTo(TARGET &target, const AddressBook &book)
        {
            for(AddressBook::ConstIterator it=book.begin();it!=book.end();++it)
                target << *static_cast<typename TARGET::ConstType *>( *it );
        }

       
    }
}

#endif
