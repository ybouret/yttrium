//! \file

#ifndef Y_Ink_Bitmap_Included
#define Y_Ink_Bitmap_Included 1


#include "y/ink/metrics.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/config/shallow.hpp"

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

            //! shared copy
            Bitmap(const Bitmap &other) noexcept;


            //! cleanup
            virtual ~Bitmap() noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
        private:
            Y_DISABLE_ASSIGN(Bitmap);
            class Code;
            Code *         code;    //!< internal memory

        public:
            BitRow * const brow;    //!< rows[h]
            const bool     dynamic; //!< if data was allocated

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            uint32_t crc32() const noexcept;

        protected:

            //! convert rows to compatible ABI
            template <typename ROW> inline
            ROW * as() noexcept { return Memory::OutOfReach::Cast<ROW,BitRow>(brow); }

            //! build all items
            void buildWith( void (*make)(void *, void*), void *args, void (*kill)(void*) );

            //! erase all items
            void eraseWith( void (*kill)(void *) ) noexcept;

        };

        Y_SHALLOW_DECL(FromBitmap); //!< alias

    }
}

#endif

