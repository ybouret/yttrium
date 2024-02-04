
//! \file

#ifndef Y_Field_Memory_Builder_Included
#define Y_Field_Memory_Builder_Included 1

#include "y/field/layout/1d.hpp"

namespace Yttrium
{
    namespace Field
    {
        //______________________________________________________________________
        //
        //
        //
        //! Memory Builder on linear space
        //
        //
        //______________________________________________________________________
        template <typename T>
        class MemoryBuilder
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
          
            //__________________________________________________________________
            //
            //! build nothing, NOEXCEPT
            //__________________________________________________________________
            inline explicit MemoryBuilder() noexcept : block(0), built(0) {}

            //__________________________________________________________________
            //
            //! build numBlocks T without argument
            /**
             \param blockAddr linear memory
             \param numBlocks number of object to build
             */
            //_________________________________________________________________
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


            //__________________________________________________________________
            //
            //
            //! build rows of a 2D space
            //
            //__________________________________________________________________
            template <
            typename META_KEY,
            typename DATATYPE>
            inline explicit MemoryBuilder(T            *    rowAddr,
                                          const size_t      numRows,
                                          const META_KEY &  rootKey,
                                          unit_t            subIndx,
                                          const Layout1D &  layout,
                                          DATATYPE       *  aliens) :
            block(rowAddr),
            built(0)
            {
                assert(Good(rowAddr,numRows));
                try {
                    const unit_t stride = layout.shift;
                    while(built<numRows) {
                        new (block+built) T(rootKey,subIndx,layout,aliens);
                        ++built;
                        ++subIndx;
                        aliens += stride;
                    }
                }
                catch(...) { clearBlocks(); throw; }
            }


            //! cleanup blocks
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

