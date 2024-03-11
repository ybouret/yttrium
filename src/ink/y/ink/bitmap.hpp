//! \file

#ifndef Y_Ink_Bitmap_Included
#define Y_Ink_Bitmap_Included 1


#include "y/ink/metrics.hpp"

namespace Yttrium
{
    namespace Ink
    {
        
        struct BitRow
        {
            void  *p;
            unit_t w;
        };

        class Bitmap : public Metrics
        {
        public:
            explicit Bitmap(const unit_t W, const unit_t H, const unsigned BPP);
            virtual ~Bitmap() noexcept;

            BitRow &       operator()(const unit_t j)       noexcept;
            const BitRow & operator()(const unit_t j) const noexcept;

            void build( void (*make)(void *, void*), void *args, void (*kill)(void*) );
            void erase( void (*kill)(void *) ) noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Bitmap);
            class Code;
            Code *         code;
            BitRow * const brow;
        };


    }
}

#endif

