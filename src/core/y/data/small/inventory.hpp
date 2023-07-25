//! \file

#ifndef Y_Data_Small_Inventory_Included
#define Y_Data_Small_Inventory_Included 1

#include "y/data/small/supply.hpp"

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
        class Inventory : public Supply
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
            virtual void release()       noexcept; //!< release all blocks
            size_t       stowage() const noexcept; //!< pool size
            

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

            virtual void *getFlat();
            virtual void  putFlat(void *) noexcept;
            
        };

    }

}

#endif

