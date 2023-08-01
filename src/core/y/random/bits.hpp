// \file

#ifndef Y_Random_Bits_Included
#define Y_Random_Bits_Included 1

#include "y/calculus/align.hpp"
#include "y/object.hpp"
#include "y/counted.hpp"
#include "y/ptr/arc.hpp"
#include <cmath>

namespace Yttrium
{
    namespace Random
    {

        //______________________________________________________________________
        //
        //
        //
        //! Random Bits interface
        //
        //
        //______________________________________________________________________
        class Bits : public Object, public Counted
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            template <typename T> class Engine;
            static const size_t         EngineBytes = 48; //!< max system size
            
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected: explicit Bits(const uint32_t umax) noexcept; //!< setup to be used within [0..umax]
        public:    virtual ~Bits()                    noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual uint32_t        next32() noexcept = 0; //!< generate next 32bit value in [0..umax]
            template <typename T> T to()     noexcept;     //!< float|double|long double|uint[8|16|32|64]_t
            template <typename T> T symm()   noexcept;     //!< float|double|long double

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________



            //! draw within range
            template <typename T> inline
            T in(const T lo, const T hi) noexcept
            {
                static const long double half(0.5);
                const long double delta = hi-lo;
                const long double start = lo;
                return static_cast<T>( floorl( start + delta * to<long double>() + half) ) ;
            }

            size_t index(const size_t n) noexcept; //!< in 1...n
            size_t leq(size_t n)         noexcept; //!< in 0..n
            size_t lt(const size_t n)    noexcept; //!< in 0..n-1, n>0
            bool   choice()              noexcept; //!< 50/50 choice
            int    intSgn()              noexcept; //!< {-1,1} 50/50

            //! integral type on exactly nbit
            template <typename T> inline T to(unsigned nbit) noexcept
            {
                assert(nbit<=sizeof(T)*8);
                if(nbit<=0) return 0;
                T res = 1;
                while(--nbit>0) { res <<= 1; if(choice()) res |= 1; }
                return res;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Bits);
            const Engine<float>       * const F;
            const Engine<double>      * const D;
            const Engine<long double> * const L;
            void *wkspF[ Y_WORDS_GEQ(EngineBytes)  ];
            void *wkspD[ Y_WORDS_GEQ(EngineBytes)  ];
            void *wkspL[ Y_WORDS_GEQ(EngineBytes)  ];
        };


        //______________________________________________________________________
        //
        //
        //
        //! Wrapper around rand() function
        //
        //
        //______________________________________________________________________
        class Rand : public Bits
        {
        public:
            explicit Rand()           noexcept; //!< initialized in [0..RAND_MAX] with system rand seed in srand()
            virtual ~Rand()           noexcept; //!< cleanup
            virtual uint32_t next32() noexcept; //!< rand()
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Rand);
        };

        typedef ArcPtr<Bits> SharedBits;
    }

}

#endif

