#include "y/ink/bitmap.hpp"
#include "y/ink/types.hpp"
#include "y/object.hpp"
#include "y/counted.hpp"
#include "y/type/nullify.hpp"
#include "y/calculus/align.hpp"
#include "y/check/crc32.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Ink
    {
        

        class Bitmap:: Code : public Object, public Counted
        {
        public:
            static inline size_t Align(const size_t x) noexcept { return Y_MEMALIGN(x); }

            explicit Code(const Metrics &metrics) : 
            Object(),
            Counted(),
            row(0),
            pix(0),
            mem(0),
            zfw(metrics.zfw)
            {
                std::cerr << "zfw: " << zfw.size << " / " << zfw.symm << std::endl;
                {
                    //__________________________________________________________
                    //
                    //
                    // compute requests
                    //
                    //__________________________________________________________
                    const size_t rowOffset = 0;
                    const size_t rowLength = sizeof(BitRow) * metrics.h;

                    const size_t pixOffset = Align(rowOffset+rowLength);
                    const size_t pixLength = metrics.n * metrics.bpp;
                    const size_t blockSize = Align(pixLength+pixOffset);

                    //__________________________________________________________
                    //
                    //
                    // allocate
                    //
                    //__________________________________________________________
                    mem  = 1;
                    {
                        uint8_t * const ptr = static_cast<uint8_t *>(MemoryModel::Instance().acquire(mem,blockSize));
                        row = static_cast<BitRow *>( Memory::OutOfReach::Addr( &ptr[rowOffset]) );
                        pix = &ptr[pixOffset];
                    }
                }

                //__________________________________________________________
                //
                //
                // link
                //
                //__________________________________________________________
                link(metrics.h,metrics.s);
                withhold();    
            }

            explicit Code(void *data, const Metrics &metrics) : 
            Object(),
            Counted(),
            row(0),
            pix(0),
            mem(0),
            zfw(metrics.zfw)
            {
                assert(0!=data);
                {
                    // compute requests
                    const size_t rowOffset = 0;
                    const size_t rowLength = sizeof(BitRow) * metrics.h;
                    const size_t blockSize = Align(rowOffset+rowLength);

                    // allocate
                    mem  = 1;
                    {
                        uint8_t * const ptr = static_cast<uint8_t *>(MemoryModel::Instance().acquire(mem,blockSize));
                        row = static_cast<BitRow *>( Memory::OutOfReach::Addr( &ptr[rowOffset]) );
                        pix = data;
                    }
                }

                // link
                link(metrics.h,metrics.s);
                withhold();
            }



            virtual ~Code() noexcept
            {
                void *ptr = Memory::OutOfReach::Addr(row);
                MemoryModel::Location().release(ptr,mem);
                row = 0;
                pix = 0;
            }

            uint32_t crc32() const noexcept
            {
                return CRC32::Of(row,mem);
            }

            BitRow        *row;
            void          *pix;
            size_t         mem;
            const ZeroFlux zfw;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
            inline void link(const unit_t height, const unit_t stride) noexcept
            {
                uint8_t *    entry  = static_cast<uint8_t *>(pix);
                for(unit_t j=0;j<height;++j, entry += stride )
                {
                    BitRow &r = row[j];
                    r.entry   = entry;
                    r.zflux   = &zfw;
                }
                
            }

        };

        uint32_t Bitmap:: crc32() const noexcept
        {
            assert(0!=code);
            return code->crc32();
        }


        Bitmap:: Bitmap(const unit_t W, const unit_t H, const unsigned BPP) :
        Metrics(W,H,BPP),
        code( new Code(*this) ),
        brow( code->row ),
        dynamic(true)
        {

        }

        Bitmap:: Bitmap(void *data, const unit_t W, const unit_t H, const unsigned BPP, const unit_t S) :
        Metrics(W,H,BPP,S),
        code( new Code(data,*this) ),
        brow(code->row),
        dynamic(false)
        {

        }


        Bitmap:: ~Bitmap() noexcept
        {
            if(code->liberate())
                Nullify(code);
            Coerce(brow) = 0;
        }

        Bitmap:: Bitmap(const Bitmap &other) noexcept :
        Metrics(other),
        code(other.code),
        brow(code->row),
        dynamic(other.dynamic)
        {
            code->withhold();
            std::cerr << "Bitmap Shared Copy of crc32=" << crc32() << " @row=" << brow << std::endl;
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
            uint8_t * const addr = static_cast<uint8_t *>(row.entry);
            for(unit_t i=num;i>0;)
            {
                void *obj = & addr[ --i * bpp ];
                zap(obj);
                memset(obj,0,bpp);
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
                uint8_t * const addr = static_cast<uint8_t *>(row.entry);
                const unit_t    size = row.zflux->size;
                while(num<size)
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



        void Bitmap:: buildWith( void (*make)(void *, void*), void *args, void (*kill)(void*) )
        {
            assert(0!=make);
            assert(0!=kill);
            unit_t j = 0;
            try {
                while(j<h)
                {
                    buildBitRow(brow[j],bpp, make, args, kill);
                    ++j;
                }
            }
            catch(...)
            {
                while(j>0)
                    eraseBitRow(brow[--j], w, bpp, kill);
                throw;
            }
        }

        void Bitmap:: eraseWith(void (*kill)(void *)) noexcept
        {
            assert(0!=kill);
            for(unit_t j=h;j>0;)
                eraseBitRow(brow[--j],w,bpp,kill);
        }

        Y_SHALLOW_IMPL(FromBitmap);

    }

}

