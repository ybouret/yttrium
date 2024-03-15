//! \file

#ifndef Y_Apex_Natural_Proto_Included
#define Y_Apex_Natural_Proto_Included 1

#include "y/apex/n/split64.hpp"
#include "y/object.hpp"
#include "y/apex/m/block.hpp"
#include "y/apex/m/batch.hpp"
#include "y/check/static.hpp"
#include "y/calculus/bit-count.hpp"
#include "y/type/capacity.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/system/wtime.hpp"
#include "y/system/exception.hpp"
#include "y/random/bits.hpp"
#include "y/type/signs.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/ptr/auto.hpp"
#include "y/fft/fft.hpp"
#include "y/type/utils.hpp"

#include <iostream>
#include <iomanip>

namespace Yttrium
{
    namespace Apex
    {

        //! helper for power of two
        Y_SHALLOW_DECL(AsShift);

        //! helper to create bytes
        Y_SHALLOW_DECL(AsByte);

        
        namespace Nexus
        {
            //__________________________________________________________________
            //
            //
            //
            //! Base class for operations prototype
            //
            //
            //__________________________________________________________________
            class Proto : public Object
            {
            public:
                static const char CallSign[]; //!< "Apex::Natural"

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
            protected:
                explicit Proto() noexcept; //!< setup

            public:
                virtual ~Proto() noexcept; //!< cleanup


                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //______________________________________________________________
                //
                //! number of bytes to hold numBits
                //______________________________________________________________
                static size_t BitsToBytes(const size_t numBits) noexcept;

                //______________________________________________________________
                //
                //! Debug check
                //______________________________________________________________
                static bool   CheckMetrics(const char  *where,
                                           const size_t nbits,
                                           const size_t bytes,
                                           const size_t words,
                                           const size_t WordSize,
                                           const size_t blockWords);
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Proto);
            };
        }


        //! in place data conversion
        struct MakeBytes
        {
            //! return &c
            static inline const uint8_t *From( uint8_t &c ) noexcept
            {
                return &c;
            }

            //! return rewritten &c
            static inline const uint8_t *From( uint16_t &c ) noexcept
            {
                const uint8_t b0 = uint8_t(c);
                const uint8_t b1 = uint8_t(c>>8);
                uint8_t      *p  = (uint8_t *)&c;
                p[0] = b0;
                p[1] = b1;
                return p;
            }

            //! return rewritten &c
            static inline const uint8_t *From( uint32_t &c ) noexcept
            {
                const uint8_t b0 = uint8_t(c);
                const uint8_t b1 = uint8_t(c>>8);
                const uint8_t b2 = uint8_t(c>>16);
                const uint8_t b3 = uint8_t(c>>24);
                uint8_t      *p  = (uint8_t *)&c;
                p[0] = b0;
                p[1] = b1;
                p[2] = b2;
                p[3] = b3;
                return p;
            }

        };

        //______________________________________________________________________
        //
        //
        //
        //! Algorithm prototypes
        //
        //
        //______________________________________________________________________
        template <typename CORE_TYPE, typename WORD_TYPE>
        class Proto : public Nexus::Proto
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const unsigned                        CoreSize  = sizeof(CORE_TYPE);             //!< alias
            static const unsigned                        WordSize  = sizeof(WORD_TYPE);             //!< alias
            static const unsigned                        WordLog2  = iLog2<WordSize>::Value ;       //!< alias
            static const unsigned                        WordBits  = WordSize << 3;                 //!< alias
            static const unsigned                        CoreBits  = CoreSize << 3;                 //!< alias
            typedef typename UnsignedInt<CoreSize>::Type CoreType;                                  //!< alias
            typedef typename UnsignedInt<WordSize>::Type WordType;                                  //!< alias
            typedef Block<WordType>                      DataType;                                  //!< alias
            typedef Split64Into<WordType>                Splitter;                                  //!< alias
            typedef typename SignedInt<CoreSize>::Type   CIntType;                                  //!< alias
            static  const WordType                       WordMaxi = UnsignedInt<WordSize>::Maximum; //!< alias
            static  const CIntType                       Radix    = CIntType(WordMaxi) + 1;         //!< alias
            static  const CoreType                       WordMask = WordMaxi;                       //!< alias
            typedef AutoPtr<Proto>                       Pointer;                                   //!< alias
            typedef Complex<double>                      cplx;                                      //!< alias

            //__________________________________________________________________
            //
            //! multiplication algorithm prototype
            //__________________________________________________________________
            typedef Proto * (*MulProc)(const WordType * const, const size_t,
                                       const WordType * const, const size_t,
                                       uint64_t *);
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t nbits; //!< exact number of bits
            const size_t bytes; //!< exact number of bytes
            const size_t words; //!< aligned to bytes
            DataType     block; //!< resources


#include "proto/cxx.hxx"
#include "proto/ops.hxx"
#include "proto/add.hxx"
#include "proto/sub.hxx"
#include "proto/cmp.hxx"
#include "proto/bit.hxx"
#include "proto/lmul.hxx"
#include "proto/fmul.hxx"
#include "proto/mul.hxx"

            //! force to zero, keep memory
            inline void ldz() noexcept
            {
                Coerce(nbits) = 0;
                Coerce(bytes) = 0;
                Coerce(words) = 0;
                block.clear();
            }


            

        private:
            Y_DISABLE_ASSIGN(Proto);
        };

    }

}

#endif

