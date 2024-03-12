
//! \file

#ifndef Y_Ink_Pixmap_Included
#define Y_Ink_Pixmap_Included 1


#include "y/ink/bitmap.hpp"
#include "y/type/args.hpp"
#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{
    namespace Ink
    {

        template <typename T>
        struct PixRow
        {
            Y_ARGS_DECL(T,Type);

            inline Type & operator[](const unit_t i) noexcept
            {
                assert(0!=entry);
                assert(0!=zflux);
                return entry[ (*zflux)(i) ];
            }

            inline ConstType & operator[](const unit_t i) const noexcept
            {
                assert(0!=entry);
                assert(0!=zflux);
                return entry[ (*zflux)(i) ];
            }


            inline size_t w() const noexcept { assert(0!=zflux); return zflux->size;  }


        private:
            MutableType *   entry;
            const ZeroFlux *zflux;
        };


        template <typename T>
        class Pixmap : public Bitmap
        {
        public:
            Y_ARGS_DECL(T,Type);
            typedef PixRow<T> RowType;


            inline explicit Pixmap(const unit_t W, const unit_t H) :
            Bitmap(W,H,sizeof(T)),
            row( Memory::OutOfReach::Cast<RowType,BitRow>(this->brow) )
            {
                buildWith(Make,0,Kill);
            }

            inline virtual ~Pixmap() noexcept
            {
                eraseWith(Kill);
            }

            inline RowType & operator[](const unit_t j) noexcept
            {
                return row[ zfh(j) ];
            }

            inline const RowType & operator[](const unit_t j) const noexcept
            {
                return row[ zfh(j) ];
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pixmap);
            RowType * const row;
            static inline void Make(void *ptr, void *)
            {
                new (ptr) T();
            }

            static inline void Kill(void *ptr) noexcept
            {
                static_cast<T*>(ptr)->~T();
            }

        };
    }

}

#endif

