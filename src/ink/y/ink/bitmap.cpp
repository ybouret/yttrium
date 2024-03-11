#include "y/ink/bitmap.hpp"
#include "y/object.hpp"
#include "y/counted.hpp"
#include "y/type/nullify.hpp"
#include "y/calculus/align.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace Yttrium
{
    namespace Ink
    {


        class Bitmap:: Code : public Object, public Counted
        {
        public:
            static inline size_t Align(const size_t x) noexcept { return Y_MEMALIGN(x); }
            typedef Memory::Dyadic MemMgr;

            explicit Code(const Metrics &metrics) : Object(), Counted(), row(0), pix(0), mem(0)
            {
                {
                    // compute requests
                    const size_t rowOffset = 0;
                    const size_t rowLength = sizeof(BitRow) * metrics.h;

                    const size_t pixOffset = Align(rowOffset+rowLength);
                    const size_t pixLength = metrics.n * metrics.b;
                    const size_t blockSize = Align(pixLength+pixOffset);

                    // allocate
                    mem  = 1;
                    {
                        uint8_t * const ptr = static_cast<uint8_t *>(MemMgr::Instance().acquire(mem,blockSize));
                        row = static_cast<BitRow *>( Memory::OutOfReach::Addr( &ptr[rowOffset]) );
                        pix = &ptr[pixOffset];
                    }
                }

                // link
                link(metrics);

                withhold();
            }

            virtual ~Code() noexcept
            {
                void *ptr = Memory::OutOfReach::Addr(row);
                MemMgr::Location().release(ptr,mem);
            }

            BitRow *row;
            void   *pix;
            size_t  mem;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
            inline void link(const Metrics &metrics) noexcept
            {
                uint8_t *entry = static_cast<uint8_t *>(pix);
                for(unit_t j=0;j<metrics.h;++j, entry += metrics.s )
                {
                    BitRow &r = row[j];
                    r.p       = entry;
                    r.w       = metrics.w;
                }
            }

        };

        Bitmap:: Bitmap(const unit_t W, const unit_t H, const unsigned BPP) :
        Metrics(W,H,BPP),
        code( new Code(*this) ),
        brow( code->row )
        {

        }

        Bitmap:: ~Bitmap() noexcept
        {
            if(code->liberate())
            {
                Nullify(code);
            }
            Coerce(brow) = 0;
        }

        BitRow & Bitmap:: operator()(const unit_t j) noexcept
        {
            assert(0!=brow);
            assert(j>=0);
            assert(j<h);
            return brow[j];
        }

        const BitRow & Bitmap:: operator()(const unit_t j) const noexcept
        {
            assert(0!=brow);
            assert(j>=0);
            assert(j<h);
            return brow[j];
        }

        //! erase num first items of row
        static inline
        void eraseBitRow(BitRow &     row,
                         unit_t       num,
                         const size_t bpp,
                         void       (*zap)(void *) ) noexcept
        {
            assert(0!=zap);
            assert(bpp>0);
            uint8_t * const addr = static_cast<uint8_t *>(row.p);
            for(unit_t i=num;i>0;)
            {
                void *obj = & addr[ --i * bpp ];
                zap(obj);
            }
        }

        //! build full row
        static inline
        void buildBitRow(BitRow &      row,
                         const size_t  bpp,
                         void        (*make)(void *,void*),
                         void *        args,
                         void        (*kill)(void *) )
        {
            assert(0!=make);
            assert(0!=kill);
            unit_t num = 0;
            try {
                uint8_t * const addr = static_cast<uint8_t *>(row.p);
                while(num<row.w)
                {
                    make(&addr[num*bpp],args);
                    ++num;
                }
            }
            catch(...)
            {
                eraseBitRow(row, num, bpp, kill);
                throw;
            }

        }



        void Bitmap:: build( void (*make)(void *, void*), void *args, void (*kill)(void*) )
        {
            assert(0!=make);
            assert(0!=kill);
            unit_t j = 0;
            try {
                while(j<h)
                {
                    buildBitRow(brow[j],b, make, args, kill);
                    ++j;
                }
            }
            catch(...)
            {
                while(j>0)
                    eraseBitRow(brow[--j], w, b, kill);
                throw;
            }
        }

        void Bitmap:: erase(void (*kill)(void *)) noexcept
        {
            assert(0!=kill);
            for(unit_t j=h;j>0;)
                eraseBitRow(brow[--j],w,b,kill);
        }


    }

}

