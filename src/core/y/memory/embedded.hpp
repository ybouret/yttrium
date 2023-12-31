
//! \file

#ifndef Y_Memory_Embedded_Included
#define Y_Memory_Embedded_Included 1

#include "y/memory/embed.hpp"
#include "y/object.hpp"

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
        class Embedded : public Object
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! allocate and link
            explicit Embedded(Embed        emb[],
                              const size_t num,
                              Allocator   &mgr);

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
        private: Allocator &   alloc; //!< persistent allocator
        public:  const  size_t bytes; //!< flat memory bytes
        private: void      *   entry; //!< flat memory entry

        };
    }

}

#endif
