
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
        //! acquire and relesease flat memory for embedded objects
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
            
            //! allocate flat memory and link, persistent embedding
            explicit Embedded(Embedding::Data &emb,
                              Allocator       &mgr);
            
            //! allocate flat memory and link, temporary embedding
            explicit Embedded(Embed * const emb,
                              const size_t  num,
                              Allocator    &mgr);

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
