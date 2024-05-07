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

            //! collect sub-histogram stored in (active) slabs
            void        collect(const Slabs &slabs);


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


            uint8_t     threshold() const; //!< minimal separation

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t total; //!< up to date population



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Histogram);
            Word data[Bins];
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

            long double computeWeightedVariance(const unsigned lower, const unsigned upper) const;
        public:
            long double fetchVarianceSeparation(const uint8_t t) const ;
        };
        

    }

}

#endif

