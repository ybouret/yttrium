
//! \file

#ifndef Y_Memory_Object_Guarded_Included
#define Y_Memory_Object_Guarded_Included 1

#include "y/memory/object/locator.hpp"

namespace Yttrium
{
    namespace Memory
    {
        //______________________________________________________________________
        //
        //
        //! guess memory Area from locator and given Query method
        //
        //______________________________________________________________________
        class ObjectGuarded
        {
        public:
            explicit ObjectGuarded(const ObjectLocator &, ObjectLocator::Query) noexcept; //!< setup
            virtual ~ObjectGuarded()                                            noexcept; //!< cleanup

            const size_t       blockSize; //!< stored block size
            const void * const blockAddr; //!< stored block address

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ObjectGuarded);
        };


        
    }

}

#endif

