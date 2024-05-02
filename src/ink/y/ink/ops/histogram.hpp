//! \file

#ifndef Y_Ink_Histogram_Included
#define Y_Ink_Histogram_Included 1

#include "y/ink/pixmap.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Ink
    {
        namespace Crux
        {
            //__________________________________________________________________
            //
            //
            //
            //! common API for Histograms
            //
            //
            //__________________________________________________________________
            class Histogram
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const unsigned BINS = 256; //!< one bin per byte
            
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                virtual ~Histogram() noexcept;
            protected:
                explicit Histogram() noexcept;
              
                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                void ldz_(void * const blocks, const size_t blockSize)                             const noexcept; //!< memset(blocks,0,blockSize*BINS)

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Histogram);
            };
        }

        //______________________________________________________________________
        //
        //
        //
        //! Histogram with integer type
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Histogram : public Crux::Histogram, public Memory::ReadOnlyBuffer
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline explicit Histogram() noexcept : data() { ldz(); }
            inline virtual ~Histogram() noexcept { ldz(); }

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            inline virtual size_t      measure() const noexcept { return sizeof(data); }
            inline virtual const void *ro_addr() const noexcept { return &data[0];     }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________


            inline void       ldz()                             noexcept { ldz_(data,sizeof(T)); } //!< reset all data
            inline T        & operator[](const uint8_t x)       noexcept { return data[x]; }       //!< access bin
            inline const  T & operator[](const uint8_t x) const noexcept { return data[x]; }       //!< access bin

            //__________________________________________________________________
            //
            //! build histogram and 8-bits gray map
            /**
             \param slabs       tesselation
             \param img         Pixmap of COLOR
             \param ColorToByte uint8_t ColorToByte(COLOR c)
             \param msk         Pixmap8 with same image metrics
             */
            //__________________________________________________________________
            template <typename COLOR, typename PROC> inline
            void make(Slabs               &slabs,
                      const Pixmap<COLOR> &img,
                      PROC                &ColorToByte,
                      Pixmap<uint8_t>     &msk)
            {
                //--------------------------------------------------------------
                // initialize
                //--------------------------------------------------------------
                ldz();

                //--------------------------------------------------------------
                // compute sub-histograms
                //--------------------------------------------------------------
                slabs(Make<COLOR,PROC>,img,ColorToByte,msk);

                //--------------------------------------------------------------
                // collect
                //--------------------------------------------------------------
                for(size_t i=slabs.simt.size();i>0;--i)
                {
                    const T * const H = slabs.simt[i].as<size_t>(BINS);
                    for(size_t j=0;j<BINS;++j)
                    {
                        data[i] += H[i];
                    }
                }
            }




        private:
            Y_DISABLE_COPY_AND_ASSIGN(Histogram);
            T data[BINS];

            //! parallel function
            template <typename COLOR, typename PROC>
            static inline
            void Make(Slab                &slab,
                      const Pixmap<COLOR> &img,
                      PROC                &ColorToByte,
                      Pixmap<uint8_t>     &msk)
            {
                assert(msk.hasSameSizesThan(img));
                T * const H = slab.ldz().as<size_t>(BINS);
                for(size_t k=slab.count();k>0;--k)
                {
                    const Ink::HSegment    s = slab.hseg[k];
                    const PixRow<COLOR>    &r = img[s.y];
                    PixRow<uint8_t>        &m = msk[s.y];
                    for(unit_t i=s.w,x=s.x;i>0;--i,++x)
                    {
                        ++H[ m[x] = ColorToByte(r(x)) ];
                    }
                }
            }
        };
    }

}

#endif

