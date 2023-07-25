
//! \file

#ifndef Y_Data_Small_Inventory_Included
#define Y_Data_Small_Inventory_Included 1

#include "y/data/pool.hpp"
#include "y/type/releasable.hpp"

namespace Yttrium
{
    namespace Small
    {
        //______________________________________________________________________
        //
        //
        //
        //! Inventory of anonymous blocks
        //
        //
        //______________________________________________________________________
        class Inventory : public Releasable
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            //! Node prototyp
            struct Node {
                Node *next; //!< for list/pool
                Node *prev; //!< for list
            };

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! initialize with bs >= sizeof(Node)
            explicit Inventory(const size_t bs) noexcept;
            virtual ~Inventory()                noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual void release()      noexcept; //!< release all blocks
            size_t       inside() const noexcept; //!< pool size
            void        *zquery()       noexcept; //!< zeroed block
            void         zstore(void *) noexcept; //!< store as Node

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t blockSize; //!< common block size
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Inventory);
            PoolOf<Node> pool;
        };

    }

}

#endif

