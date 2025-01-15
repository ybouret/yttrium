
//! \file

#ifndef Y_Apex_Block_Ptr_Included
#define Y_Apex_Block_Ptr_Included 1

#include "y/apex/block.hpp"
#include "y/ptr/core.hpp"

namespace Yttrium
{
    namespace Apex
    {

        //______________________________________________________________________
        //
        //
        //
        //! reference counted pointer, using factory to release
        //
        //
        //______________________________________________________________________
        class BlockPtr : public Core::Ptr
        {
        public:
            BlockPtr(Block * const block) noexcept; //!< setup, withhold
            BlockPtr(const BlockPtr &)    noexcept; //!< copy,  withold
            virtual ~BlockPtr()           noexcept; //!< liberate, may release

            Block *       operator->()       noexcept; //!< access
            Block &       operator*()        noexcept; //!< access
            const Block * operator->() const noexcept; //!< access
            const Block & operator*()  const noexcept; //!< access


        private:
            Y_DISABLE_ASSIGN(BlockPtr);
            Block * const host;
        };
    }

}

#endif

