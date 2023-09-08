
//! \file

#ifndef Y_Memory_Object_Locator_Included
#define Y_Memory_Object_Locator_Included 1

#include "y/config/starting.hpp"
#include "y/ostream-proto.hpp"

namespace Yttrium
{
    namespace Memory
    {
        class Arena;
        class Chunk;
        class Strap;

        //______________________________________________________________________
        //
        //
        //! find memory information of a block of memory allocated by object
        //
        //______________________________________________________________________
        class ObjectLocator
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit ObjectLocator(const void *blockAddr); //!< setup
            virtual ~ObjectLocator() noexcept;             //!< cleanup
            Y_OSTREAM_PROTO(ObjectLocator);                //!< display

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const void  * const where; //!< block address
            const Arena * const arena; //!< if small object
            const Chunk * const chunk; //!< if small object
            const Strap * const strap; //!< if large object
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ObjectLocator);
        };

    }

}

#endif
