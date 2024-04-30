//! \file

#ifndef Y_Chemical_Types_Included
#define Y_Chemical_Types_Included 1

#include "y/mkl/api.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/mkl/antelope/mul.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/associative/address-book.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef double                      real_t;      //!< hardware real
        typedef XReal<real_t>               xreal_t;     //!< software real
        typedef MKL::Antelope::Add<xreal_t> XAdd;        //!< alias
        typedef MKL::Antelope::Mul<xreal_t> XMul;        //!< alias
        typedef Memory::Dyadic              MemoryModel; //!< alias
        


        //! helper to load a Small List from addressess
        template <typename TARGET>
        void SendBookTo(TARGET &target, const AddressBook &book)
        {
            for(AddressBook::ConstIterator it=book.begin();it!=book.end();++it)
                target << *static_cast<typename TARGET::ConstType *>( *it );
        }

    }
}

#endif
