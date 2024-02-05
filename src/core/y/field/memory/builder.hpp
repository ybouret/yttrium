
//! \file

#ifndef Y_Field_Memory_Builder_Included
#define Y_Field_Memory_Builder_Included 1

#include "y/field/layout/1d.hpp"
#include "y/field/layout/2d.hpp"
#include "y/field/layout/3d.hpp"

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
                                          const Format1D &  space,
                                          DATATYPE       *  alien) :
            block(rowAddr),
            built(0)
            {
                assert(Good(rowAddr,numRows));
                typedef T Row;
                try {
                    const unit_t stride = space->shift;
                    while(built<numRows) {
                        new (block+built) Row(rootKey,subIndx,space,alien);
                        ++built;
                        ++subIndx;
                        alien += stride;
                    }
                }
                catch(...) { clearBlocks(); throw; }
            }

            //__________________________________________________________________
            //
            //
            //! build slices of a 3D space
            //
            //__________________________________________________________________
            template <
            typename META_KEY,
            typename DATATYPE,
            typename ROW_TYPE>
            inline explicit MemoryBuilder(T *             sliceAddr,
                                          const size_t    numSlices,
                                          const META_KEY &rootKey,
                                          unit_t          subIndx,
                                          const Format2D &space2D,
                                          const Format1D &space1D,
                                          ROW_TYPE       *alienRows,
                                          DATATYPE       *alienData) :
            block(sliceAddr),
            built(0)
            {
                assert(Good(sliceAddr,numSlices));
                typedef T Slice;
                try {
                    const size_t RowsPerSlice = space2D->width.y;
                    const size_t DataPerSlice = space2D->items;
                    while(built<numSlices) {
                        new (block+built) Slice(rootKey,subIndx,space2D,space1D,alienRows,alienData);
                        ++built;
                        ++subIndx;
                        alienRows += RowsPerSlice;
                        alienData += DataPerSlice;
                    }
                 }
                catch(...) { clearBlocks(); throw; }
            }


            //__________________________________________________________________
            //
            //
            //! build volumes of a 4D space
            //
            //__________________________________________________________________
            template <
            typename META_KEY,
            typename DATATYPE,
            typename ROW_TYPE,
            typename SLC_TYPE>
            inline explicit MemoryBuilder(T *             volAddr,
                                          const size_t    numVols,
                                          const META_KEY &rootKey,
                                          unit_t          subIndx,
                                          const Format3D &space3D,
                                          const Format2D &space2D,
                                          const Format1D &space1D,
                                          SLC_TYPE       *alienSlcs,
                                          ROW_TYPE       *alienRows,
                                          DATATYPE       *alienData) :
            block(volAddr),
            built(0)
            {
                assert(Good(volAddr,numVols));
                typedef T Volume;
                try {
                    const size_t SlcsPerVolume = space3D->width.z;
                    const size_t RowsPerSlice  = space3D->width.y;
                    const size_t RowsPerVolume = SlcsPerVolume * RowsPerSlice;
                    const size_t DataPerVolume = space3D->items;
                    while(built<numVols) {
                        new (block+built) Volume(rootKey,subIndx,space3D,space2D,space1D,alienSlcs,alienRows,alienData);
                        ++built;
                        ++subIndx;
                        alienSlcs += SlcsPerVolume;
                        alienRows += RowsPerVolume;
                        alienData += DataPerVolume;
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

