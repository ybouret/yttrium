//! \file

#ifndef Y_Ink_Bitmap_Included
#define Y_Ink_Bitmap_Included 1


#include "y/ink/metrics.hpp"
#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{
    namespace Ink
    {

        //______________________________________________________________________
        //
        //
        //
        //! anonymous row of data
        //
        //
        //______________________________________________________________________
        struct BitRow
        {
            void           *entry; //!< address of first item in row
            const ZeroFlux *zflux; //!< width zero flux
        };

        //______________________________________________________________________
        //
        //
        //! anonymous bitmap
        //
        //______________________________________________________________________
        class Bitmap : public Metrics
        {
        public:
            explicit Bitmap(const unit_t W, const unit_t H, const unsigned BPP);
            explicit Bitmap(void *data, const unit_t W, const unit_t H, const unsigned BPP, const unit_t S);
            virtual ~Bitmap() noexcept;

            void buildWith( void (*make)(void *, void*), void *args, void (*kill)(void*) );
            void eraseWith( void (*kill)(void *) ) noexcept;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Bitmap);
            class Code;
            Code *         code;
        protected:
            BitRow * const brow;
        public:
            const bool     dynamic;
        protected:
            template <typename ROW> inline
            ROW * as() noexcept { return Memory::OutOfReach::Cast<ROW,BitRow>(brow); }
        };


    }
}

#endif

