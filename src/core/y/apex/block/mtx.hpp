

//! \file

#ifndef Y_Apex_Mutex_Ptr_Included
#define Y_Apex_Mutex_Ptr_Included 1

#include "y/apex/type/mutex.hpp"
#include "y/ptr/core.hpp"

namespace Yttrium
{
    namespace Apex
    {

        //______________________________________________________________________
        //
        //
        //
        //! reference counted mutex, using factory to release
        //
        //
        //______________________________________________________________________
        class MutexPtr : public Core::Ptr
        {
        public:
            MutexPtr(Mutex * const)       noexcept; //!< setup, withhold
            MutexPtr(const MutexPtr &)    noexcept; //!< copy,  withold
            virtual ~MutexPtr()           noexcept; //!< liberate, may release

            MutexType *       operator->()       noexcept; //!< access
            MutexType &       operator*()        noexcept; //!< access
            const MutexType * operator->() const noexcept; //!< access
            const MutexType & operator*()  const noexcept; //!< access


        private:
            Y_DISABLE_ASSIGN(MutexPtr);
            Mutex * const host;
        };
    }

}

#endif

