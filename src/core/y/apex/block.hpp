

//! \file

#ifndef Y_Apex_Block_Included
#define Y_Apex_Block_Included 1

#include "y/apex/jig/jigs.hpp"
#include "y/apex/block/data.hpp"
#include "y/data/list/cxx.hpp"
#include "y/data/pool/cxx.hpp"

namespace Yttrium
{
    namespace Apex
    {
        class Block : public DataBlock, public Jigs
        {
        public:

            typedef CxxListOf<Block> List;
            typedef CxxPoolOf<Block> Pool;

            static const size_t One = 1;

            explicit Block(const unsigned _shift);
            virtual ~Block() noexcept;
            Y_OSTREAM_PROTO(Block);


            //! set all to zero, keep plan for futrure use
            void ldz() noexcept;

            //! set curr and dull from plan
            void relink() noexcept;

            //! sync dull after manual setting of curr
            void syncDull() noexcept;

            //! compute bits from current plan and update dull
            void sync() noexcept;

            //! change representation
            void to(const Plan target) noexcept;

            const size_t  &bytes;
            Block *        next;                //!< for list/pool
            Block *        prev;                //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Block);
        };

      

    }

}

#endif

