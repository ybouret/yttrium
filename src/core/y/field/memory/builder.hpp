
//! \file

#ifndef Y_Field_Memory_Builder_Included
#define Y_Field_Memory_Builder_Included 1

#include "y/field/layout/1d.hpp"

namespace Yttrium
{
    namespace Field
    {
        template <typename T>
        class MemoryBuilder
        {
        public:
            inline explicit MemoryBuilder(T           *blockAddr,
                                          const size_t numBlocks) :
            block(blockAddr),
            built(0)
            {
                assert(Good(blockAddr,numBlocks));
                try {
                    while(built<numBlocks) {
                        new (block+built) T();
                        ++built;
                    }
                }
                catch(...) { clearBlocks(); throw; }
            }

            //! build nothing, NOEXCEPT
            inline explicit MemoryBuilder() noexcept : block(0), built(0) {}

            template <
            typename META_KEY,
            typename DATATYPE>
            inline explicit MemoryBuilder(T            *    blockAddr,
                                          const size_t      numBlocks,
                                          const META_KEY &  rootKey,
                                          unit_t            subIndx,
                                          const Layout1D &  layout,
                                          DATATYPE       *  aliens) :
            block(blockAddr),
            built(0)
            {
                assert(Good(blockAddr,numBlocks));
                try {
                    while(built<numBlocks) {
                        new (block+built) T(rootKey,subIndx,layout,aliens);
                        ++built;
                        ++subIndx;
                        aliens += layout.items;
                    }
                }
                catch(...) { clearBlocks(); throw; }
            }

            inline virtual ~MemoryBuilder() noexcept
            {
                clearBlocks();
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MemoryBuilder);
            T     *block;
            size_t built;

            inline void clearBlocks() noexcept
            {
                while(built>0) Memory::OutOfReach::Naught( &block[--built] );
                block = 0;
            }
        };
    }

}

#endif

