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
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

        protected:

            //! setup/allocate data+rows WxHxBPP
            explicit Bitmap(const unit_t W, const unit_t H, const unsigned BPP);

            //! setup/allocate rows WxHxBPP
            /**
             \param data user's data
             \param W    width
             \param H    height
             \param BPP  bytes per pixel
             \param S    stride in bytes
             */
            explicit Bitmap(void *data, const unit_t W, const unit_t H, const unsigned BPP, const unit_t S);

        public:
            //! cleanup
            virtual ~Bitmap() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! build all items
            void buildWith( void (*make)(void *, void*), void *args, void (*kill)(void*) );

            //! erase all items
            void eraseWith( void (*kill)(void *) ) noexcept;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Bitmap);
            class Code;
            Code *         code; //!< internal memory
        protected:
            BitRow * const brow; //!< rows[h]
        public:
            const bool     dynamic; //!< if data was allocated
        protected:
            //! convert rows
            template <typename ROW> inline
            ROW * as() noexcept { return Memory::OutOfReach::Cast<ROW,BitRow>(brow); }
        };


    }
}

#endif
