

//! \file

#ifndef Y_Apex_Blocks_Included
#define Y_Apex_Blocks_Included 1

#include "y/apex/block.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    namespace Apex
    {
        class Blocks : public Proxy<const Block::Pool>
        {
        public:
            explicit Blocks(const unsigned _shift) noexcept;
            virtual ~Blocks() noexcept;

            Block * query();
            void    store(Block * const block) noexcept;
            void    gc() noexcept;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blocks);
            Y_PROXY_DECL();
            Block::Pool my;

        public:
            const unsigned shift;

        };

    }

}

#endif


