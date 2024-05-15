//! \file

#ifndef Y_Ink_Block_Included
#define Y_Ink_Block_Included 1

#include "y/ink/pixmap.hpp"

namespace Yttrium
{
    namespace Ink
    {

        //______________________________________________________________________
        //
        //
        //
        //! Block class to apply block operations
        //
        //
        //______________________________________________________________________
        template <size_t DeltaX, size_t DeltaY>
        class Block
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const unit_t X  = DeltaX; //!< max X offset
            static const unit_t Y  = DeltaY; //!< max Y offset
            static const unit_t X0 = -X;     //!< min X offset
            static const unit_t Y0 = -Y;     //!< min Y offset
            static const unit_t W  = X*2+1;  //!< width
            static const unit_t H  = Y*2+1;  //!< height
            static const size_t N  = W*H;    //!< items per block

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Block() noexcept {} //!< setup
            virtual ~Block() noexcept {} //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! helper to apply operation on pixmap
            template <typename TARGET, typename SOURCE, typename PROC> inline
            void operator()(Slabs &slabs, Pixmap<TARGET> &target, PROC &proc, const Pixmap<SOURCE> &source) const
            {
                slabs(Apply<TARGET,PROC,SOURCE>,target,*this,proc,source);
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Block);

            template <typename TARGET, typename PROC, typename SOURCE> static inline
            void Apply(Slab                 &slab,
                       Pixmap<TARGET>       &target,
                       const Block          &block,
                       PROC                 &proc,
                       const Pixmap<SOURCE> &source)
            {
                //--------------------------------------------------------------
                // local memory for all slab operations
                //--------------------------------------------------------------
                static const size_t Required = N * sizeof(SOURCE);
                static const size_t NumWords = Y_WORDS_GEQ(Required);
                void   *       wksp[NumWords];
                SOURCE * const data = static_cast<SOURCE *>( Memory::OutOfReach::Addr(wksp) );

                //--------------------------------------------------------------
                // loop over the slab
                //--------------------------------------------------------------
                for(size_t k=slab.count();k>0;--k)
                {
                    const HSegment  seg = slab[k];
                    const unit_t    y   = seg.y;
                    PixRow<TARGET> &tgt = target[y];
                    Coord           pos(seg.x,y);
                    for(unit_t i=seg.w;i>0;--i,++pos.x)
                    {
                        block.load(data,source,pos); // load all data from source
                        proc(tgt[pos.x],data);       // put result in target
                    }
                }
            }

            // load target[N] wit data around origin
            template <typename T> inline
            void load(T               *target,
                      const Pixmap<T> &source,
                      const Coord      origin) const
            {
                for(unit_t y=Y;y>=Y0;--y)
                {
                    const PixRow<T> &row = source[origin.y+y];
                    unit_t           x   = origin.x+X0;
                    for(unit_t i=W;i>0;--i,++x)
                    {
                        *(target++) = row[x];
                    }
                }
            }
        };
    }

}

#endif

