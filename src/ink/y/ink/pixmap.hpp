
//! \file

#ifndef Y_Ink_Pixmap_Included
#define Y_Ink_Pixmap_Included 1


#include "y/ink/bitmap.hpp"
#include "y/type/args.hpp"
#include <iostream>

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

            //! access item
            inline Type & operator[](const unit_t i) noexcept
            {
                assert(0!=entry);
                assert(0!=zflux);
                return entry[ (*zflux)[i] ];
            }

            //! access const item
            inline ConstType & operator[](const unit_t i) const noexcept
            {
                assert(0!=entry);
                assert(0!=zflux);
                return entry[ (*zflux)[i] ];
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
            inline explicit Pixmap(T * data, const unit_t W, const unit_t H, const unit_t S) :
            Bitmap(data,W,H,sizeof(T),S*sizeof(T)),
            row( this->as<RowType>() )
            {
                assert(!dynamic);
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

            //! access Row
            inline RowType & operator[](const unit_t j) noexcept
            {
                return row[ zfh[j] ];
            }

            //! access const Row
            inline const RowType & operator[](const unit_t j) const noexcept
            {
                return row[ zfh[j] ];
            }

            //! display
            inline friend std::ostream & operator<<(std::ostream &os, const Pixmap &pxm)
            {
                os << '[' << pxm[0];
                for(unit_t j=1;j<pxm.h;++j)
                {
                    os << ';' << pxm[j];
                }
                os << ']';
                return os;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pixmap);
            RowType * const row;
            
            static inline void Make(void *ptr, void *)  { new (ptr) T(); }
            static inline void Kill(void *ptr) noexcept { static_cast<T*>(ptr)->~T(); }

        };
    }

}

#endif
