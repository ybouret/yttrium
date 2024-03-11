#include "y/ink/bitmap.hpp"
#include "y/type/args.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

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

        template <typename T>
        struct IOXX
        {
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

#include "y/string.hpp"

Y_UTEST(bitmap)
{
    
    {
        int                 pixel[] = { 1, 2, 3, 4 };
        const unit_t        count   = sizeof(pixel)/sizeof(pixel[0]);
        const Ink::ZeroFlux zf(count);
        Ink::BitRow         row_    = { pixel, &zf };
        Ink::PixRow<int>   &row     = Memory::OutOfReach::Conv< Ink::PixRow<int>, Ink::BitRow>(row_);

        std::cerr << " w = " << row.w() << std::endl;


    }
    {
        Ink::Bitmap bmp1(10,10,1);
        bmp1.buildWith(Ink::IOXX<uint8_t>::Make,0, Ink::IOXX<uint8_t>::Kill);
    }

    {
        Ink::Bitmap bmpS(8,6,sizeof(String));
        bmpS.buildWith(Ink::IOXX<String>::Make,0, Ink::IOXX<String>::Kill);
        bmpS.eraseWith(Ink::IOXX<String>::Kill);
    }

    Ink::Pixmap<String> pix(100,23);

    const Ink::Pixmap<String> & cst = pix;

    for(unit_t j=0;j<pix.h;++j)
    {
        for(unit_t i=0;i<pix.w;++i)
        {
            pix[j][i] = "Hello";
            Y_ASSERT("Hello"==cst[j][i]);
        }

    }
}
Y_UDONE()
