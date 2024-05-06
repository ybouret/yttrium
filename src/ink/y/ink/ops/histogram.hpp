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

            long double compute(const unsigned lower, const unsigned upper) const
            {
                size_t num = 0;
                size_t sum = 0;
                for(size_t i=lower;i<upper;++i)
                {
                    const size_t n = data[i];
                    num += n;
                    sum += n * i;
                }
                if(num>0)
                {
                    const long double proba    = static_cast<long double>(num)/total;
                    const long double average  = static_cast<long double>(sum)/num;
                    long double       variance = 0;
                    for(size_t i=lower;i<upper;++i)
                    {
                        const long double n = static_cast<long double>(data[i]);
                        const long double d = static_cast<long double>(i) - average;
                        variance += n * d*d;
                    }
                    variance /= num;
                    return proba * variance;
                }
                else
                {
                    return 0;
                }
            }

            long double separation(const uint8_t t) const {
                return compute(0,t) + compute(t,255);
            }

            uint8_t threshold() const
            {
                long double sprev = separation(0);
                for(unsigned i=1;i<=255;++i)
                {
                    const long double scurr = separation(i);
                    if(scurr>=sprev) return i-1;
                    sprev=scurr;
                }
                return 255;
            }


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t total;



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
        };
        

    }

}

#endif

