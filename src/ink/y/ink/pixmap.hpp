//! \file

#ifndef Y_Ink_Pixmap_Included
#define Y_Ink_Pixmap_Included 1

#include "y/ink/bitmap.hpp"
#include "y/ink/parallel/slabs.hpp"

namespace Yttrium
{
    namespace Ink
    {

        //______________________________________________________________________
        //
        //
        //
        //! Row for Pixmap
        //
        //
        //______________________________________________________________________
        template <typename T>
        struct PixRow
        {
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_DECL(T,Type); //!< aliases

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! mutable access: direct
            inline Type & operator[](const unit_t i) noexcept
            {
                assert(0!=entry);
                assert(0!=zflux);
                assert(i>=0);
                assert(i<zflux->size);
                return entry[i];
            }

            //! const access: zero_flux
            inline ConstType & operator[](const unit_t i) const noexcept
            {
                assert(0!=entry);
                assert(0!=zflux);
                return entry[ (*zflux)[i] ];
            }

            //! const (faster) direct access
            inline ConstType & operator()(const unit_t i) const noexcept
            {
                assert(0!=entry);
                assert(0!=zflux);
                assert(i>=0);
                assert(i<zflux->size);
                return entry[i];
            }

            //! display
            inline friend std::ostream & operator<<(std::ostream &os, const PixRow &r)
            {
                assert(0!=r.entry);
                assert(0!=r.zflux);
                os << r.entry[0];
                for(unit_t i=1;i<r.zflux->size;++i)
                {
                    os << ' ' << r.entry[i];
                }
                return os;
            }

            //! fetch zflux size a.k.a width
            inline unit_t w() const noexcept { assert(0!=zflux); return zflux->size; }

        private:
            MutableType *   entry;
            const ZeroFlux *zflux;
        };


        
        //______________________________________________________________________
        //
        //
        //
        //! Pixmap
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Pixmap : public Bitmap
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_DECL(T,Type);        //!< aliases
            typedef PixRow<T> RowType;  //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! zeroed WxH
            inline explicit Pixmap(const unit_t W, const unit_t H) :
            Bitmap(W,H,sizeof(T)),
            row( this->as<RowType>() )
            {
                buildWith(Make,0,Kill);
            }
            

            //! based on user's data
            /**
             \param data user's data
             \param W    width
             \param H    height
             \param S    stride in items, S>=W
             */
            inline explicit Pixmap(T * const data, const unit_t W, const unit_t H, const unit_t S) :
            Bitmap(data,W,H,sizeof(T),S*sizeof(T)),
            row( this->as<RowType>() )
            {
                assert(!dynamic);
            }

            //! full shared copy
            inline   Pixmap(const Pixmap &pixmap) noexcept :
            Bitmap(pixmap),
            row( this->as<RowType>() )
            {

            }

            //! shared copy
            inline Pixmap(const FromBitmap_ &, const Bitmap &bmp) noexcept :
            Bitmap(bmp),
            row( this->as<RowType>() )
            {
                assert(bmp.bpp==bpp);
            }

            //! build with parallel transformation
            template <typename PROC, typename U> inline
            Pixmap(Slabs &slabs, PROC &proc, const Pixmap<U> &src) :
            Bitmap(w,h,bpp),
            row( this->as<RowType>() )
            {
                slabs(*this,proc,src);
            }


            //! cleanup
            inline virtual ~Pixmap() noexcept
            {
                if(dynamic)
                    eraseWith(Kill);
            }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! mutable access: direct
            inline RowType & operator[](const unit_t j) noexcept
            {
                assert(j>=0);
                assert(j<h);
                return row[j];     
            }

            //! const access: zeroflux
            inline const RowType & operator[](const unit_t j) const noexcept
            {
                return row[ zfh[j] ];
            }


            //! direct (faster) const access
            inline const RowType & operator()(const unit_t j) const noexcept
            {
                assert(j>=0);
                assert(j<h);
                return row[j];
            }

            //! display
            inline friend std::ostream & operator<<(std::ostream &os, const Pixmap &pxm)
            {
                for(unit_t j=0;j<pxm.h-1;++j)
                {
                    os << ' ' << pxm[j] << '\n';
                }
                os <<' '<< pxm[pxm.h-1];
                return os;
            }

            

        private:
            Y_DISABLE_ASSIGN(Pixmap);
            RowType * const row;

            static inline void Make(void *ptr, void *)  { new (ptr) MutableType(); }
            static inline void Kill(void *ptr) noexcept { Destruct( static_cast<MutableType*>(ptr) ); }

        };
    }

}

#endif

