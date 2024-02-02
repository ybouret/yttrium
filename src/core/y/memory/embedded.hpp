
//! \file

#ifndef Y_Memory_Embedded_Included
#define Y_Memory_Embedded_Included 1

#include "y/memory/embedding/data.hpp"

namespace Yttrium
{
    namespace Memory
    {
        //______________________________________________________________________
        //
        //
        //
        //! Resource acquisition for embedded objetcs
        //
        //
        //______________________________________________________________________
        class Embedded
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            
            //! allocate and link
            explicit Embedded(Embedding::Data &emb,
                              Allocator       &mgr);

            //! allocate nothing, NOEXCEPT
            explicit Embedded() noexcept;

            //! release flat memory
            virtual ~Embedded() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Embedded);

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
        private: Allocator * const alloc; //!< persistent allocator
        public:  const  size_t     bytes; //!< flat memory bytes
        private: void      * const entry; //!< flat memory entry
        };
        
    }

}

#endif
