
//! \file

#ifndef Y_Apex_Block_Ptr_Included
#define Y_Apex_Block_Ptr_Included 1

#include "y/apex/block.hpp"
#include "y/ptr/core.hpp"

namespace Yttrium
{
    namespace Apex
    {
        class BlockPtr : public Core::Ptr
        {
        public:
            BlockPtr(Block * const block) noexcept;
            BlockPtr(const BlockPtr &)     noexcept;
            virtual ~BlockPtr() noexcept;

            Block *       operator->() noexcept;
            const Block * operator->() const noexcept;
            Block &       operator*()  noexcept;
            const Block & operator*()  const noexcept;


        private:
            Y_DISABLE_ASSIGN(BlockPtr);
            Block * const host;
        };
    }

}

#endif

