

//! \file

#ifndef Y_Apex_Blocks_Included
#define Y_Apex_Blocks_Included 1

#include "y/apex/block.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    namespace Apex
    {
        //______________________________________________________________________
        //
        //
        //
        //! pool of blocks with same shift/range
        //
        //
        //______________________________________________________________________
        class Blocks : public Proxy<const Block::Pool>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Blocks(const unsigned _shift) noexcept; //!< setup
            virtual ~Blocks() noexcept;                      //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Block * query();                             //!< cached/new block
            void    store(Block * const block) noexcept; //!< store and cleanup
            void    gc(const size_t cycles)    noexcept; //!< apply GC
            Block *tryQuery()                  noexcept; //!< try got someting
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blocks);
            Y_PROXY_DECL();
            Block::Pool my;

        public:
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const unsigned shift; //!< common shift

        };

    }

}

#endif


