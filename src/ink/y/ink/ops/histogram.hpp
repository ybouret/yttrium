//! \file

#ifndef Y_Ink_Histogram_Included
#define Y_Ink_Histogram_Included 1

#include "y/ink/pixmap.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Ink
    {

        //______________________________________________________________________
        //
        //
        //
        //! Histogram
        //
        //
        //______________________________________________________________________
        class Histogram : public Memory::ReadOnlyBuffer
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const unsigned Bins = 256; //!< 8bits bins
            typedef size_t        Word;       //!< data per bin

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Histogram() noexcept; //!< setup
            virtual ~Histogram() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual size_t      measure() const noexcept; //!< internal bytes
            virtual const void *ro_addr() const noexcept; //!< internal data

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Word operator[](const uint8_t) const noexcept;            //!< access
            void ldz()                           noexcept;            //!< reset
            void add(const Word * const h)       noexcept;            //!< update
            bool find(uint8_t &lower, uint8_t &upper) const noexcept; //!< non-zero indices
            
            //! write content to output
            void write(OutputStream &fp) const;

            //! save content to file
            template <typename FILENAME>
            inline void save(const FILENAME & fileName) const {
                OutputFile fp(fileName); write(fp);
            }

            //! zeroed workpsace
            static Word * BinsFrom(Slab &slab);


            //! load values from mask
            void load(Slabs &slabs, const Pixmap<uint8_t> &mask);
          
            //! load values from source, stored into mask
            template <typename COLOR, typename PROC> inline
            void load(Slabs               &slabs,
                      const Pixmap<COLOR> &source,
                      PROC                &ColorToByte,
                      Pixmap<uint8_t>     &mask)
            {
                ldz();
                slabs( Make<COLOR,PROC>, source, ColorToByte, mask);
                collect(slabs);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Histogram);
            Word data[Bins];
            void        collect(const Slabs &slabs);
            static void LoadDirectly(Slab &slab, const Pixmap<uint8_t> &mask);

            //! parallel function
            template <typename COLOR, typename PROC> static inline
            void Make(Slab                &slab,
                      const Pixmap<COLOR> &img,
                      PROC                &c2b,
                      Pixmap<uint8_t>     &msk)
            {
                assert(msk.hasSameSizesThan(img));
                Word * const H = slab.ldz().as<Word>(Bins);
                for(size_t k=slab.count();k>0;--k)
                {
                    const Ink::HSegment    s = slab.hseg[k];
                    const PixRow<COLOR>    &r = img(s.y);
                    PixRow<uint8_t>        &m = msk[s.y];
                    for(unit_t i=s.w,x=s.x;i>0;--i,++x)
                    {
                        ++H[ m[x] = c2b(r(x)) ];
                    }
                }
            }
        };
        
#if 0
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
                void ldz_(void * const blocks, const size_t blockSize) const noexcept; //!< memset(blocks,0,blockSize*BINS)

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Histogram);
            public:
                const uint8_t lower; //!< lower not zero bin
                const uint8_t upper; //!< upper not zero bin
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
            inline const  T & operator[](const uint8_t x) const noexcept { return data[x]; }       //!< access bin

            //! write content to output
            inline void write(OutputStream &fp) const
            {
                for(unsigned j=0;j<BINS;++j)
                {
                    fp("%u %g\n",j,double(data[j]));
                }
            }

            //! save content to file
            template <typename FILENAME>
            inline void save(const FILENAME & fileName) const
            {
                OutputFile fp(fileName);
                write(fp);
            }
            
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
                    const Slab &slab = slabs.simt[i];
                    if(slab.count()<=0) continue;
                    const T * const H = slab.as<size_t>(BINS);
                    for(unsigned j=0;j<BINS;++j)
                        data[j] += H[j];
                }

                for(unsigned j=0;j<BINS;++j)
                {
                    if(data[j]<=0) continue;
                    Coerce(lower) = uint8_t(j);
                    break;
                }

                for(int j=BINS-1;j>0;--j)
                {
                    if(data[j]<=0) continue;
                    Coerce(upper) = uint8_t(j);
                    break;
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
#endif
    }

}

#endif

