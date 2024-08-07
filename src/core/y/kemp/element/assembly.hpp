//! \file

#ifndef Y_Kemp_Assembly_Included
#define Y_Kemp_Assembly_Included 1

#include "y/kemp/element/pull64.hpp"
#include "y/kemp/element/push64.hpp"
#include "y/config/shallow.hpp"
#include "y/calculus/align.hpp"
#include "y/tow/api.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/calculus/bit-count.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/type/signs.hpp"
#include <iostream>
#include <iomanip>
#include <cstring>

namespace Yttrium
{
    namespace Kemp
    {

        Y_SHALLOW_DECL(AsBits); //!< alias


        //______________________________________________________________________
        //
        //
        //
        //! Assembly of items, the number of positive ones is handled by Element
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Assembly
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef T             WordType;                              //!< alias
            static const unsigned WordSize     = sizeof(WordType);       //!< alias
            static const unsigned WordBits     = WordSize << 3;          //!< alias
            static const unsigned Log2WordBits = iLog2<WordBits>::Value; //!< alias

            //! bits to number of positive items
            static inline size_t BitsToPositive(const size_t bits) noexcept {
                return (Y_ALIGN_LN2(Log2WordBits,bits)) >> Log2WordBits;
            }

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! build from a PERSISTENT QWORD
            inline Assembly(uint64_t &qw) noexcept :
            capacity( sizeof(uint64_t) / WordSize ),
            positive( Push64::To( (T*)&qw, qw)    ),
            item( static_cast<T*>(Memory::OutOfReach::Addr( & qw )) )
            {
                assert(0!=item);
                assert(capacity>0);
                assert(positive<=capacity);
                assert(IsPowerOfTwo(capacity));
            }

            //! build from PERSISTENT user data
            inline Assembly(void * const       data,
                            const size_t       capa,
                            const size_t       npos) noexcept :
            capacity(capa),
            positive(npos),
            item( static_cast<T *>(data) )
            {
                assert(0!=item);
                assert(capacity>0);
                assert(positive<=capacity);
                assert(IsPowerOfTwo(capacity));
            }

            //! build from PERSISTENT user data and given bits
            inline Assembly(void * const       data,
                            const size_t       capa,
                            const size_t       bits,
                            const AsBits_      &) noexcept :
            capacity(capa),
            positive( BitsToPositive(bits) ),
            item( static_cast<T *>(data) )
            {
                assert(0!=item);
                assert(capacity>0);
                assert(positive<=capacity);
                assert(IsPowerOfTwo(capacity));
            }

            //! cleanup
            inline ~Assembly() noexcept
            {
                Coerce(capacity) = 0;
                positive         = 0;
                Coerce(item)     = 0;
            }

            //! detailed output
            inline friend std::ostream & operator<<(std::ostream &os, const Assembly &self)
            {
                os << '[' << std::setw(3) << self.positive << '/' << std::setw(3) << self.capacity <<  ']' << '@' << (const void *) self.item;
                Hexadecimal::Display(os << '=',self.item,self.positive);
                return os;
            }

            //! set to 0 => 0 bits for Elemebnt
            inline void ldz() noexcept
            {
                assert(Memory::OutOfReach::Are0(item+positive,(capacity-positive)*sizeof(T)));
                memset(item,0,positive*sizeof(T));
                positive=0;
            }

            //! set to 1 => 1 bits for Element
            inline void ld1() noexcept
            {
                assert(Memory::OutOfReach::Are0(item+positive,(capacity-positive)*sizeof(T)));
                memset(item,0,positive*sizeof(T));
                positive=1;
                item[0] = 1;
            }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
          
            //! test if shrinkable with capacity*WordSize > minBytes
            /**
             can happen after subtraction of division
             */
            inline bool mayShrinkAbove(const size_t minBytes) const noexcept
            {
                const bool flag = ( (capacity*WordSize>minBytes) && (positive < (capacity>>1)) );
                //if(flag) std::cerr << "May Shrink(positive=" << positive << "/capacity=" << capacity << ")!" << std::endl;
                return flag;
            }

            //! transmogrify from another assembly
            template <typename U> inline
            void load(const Assembly<U> &source) noexcept
            {
                const size_t cycles = TOW::API<T,U>::Cycles(capacity,source.capacity);
                TOW::Transmute(item,source.item,cycles);
            }

            //! checking number of bits with most significant word
            inline size_t updateBits() noexcept
            {
                while(positive>0)
                {
                    const size_t msi = positive-1;
                    const T      top = item[msi];
                    if(top>0)
                        return (msi<<Log2WordBits) + BitCount::For(top);
                    positive = msi;
                }
                assert(0==positive);
                return 0;
            }

            //! update positive items from bits
            inline void updatePositive(const size_t bits) noexcept
            {
                assert(bits<=capacity*WordBits);
                positive = BitsToPositive(bits);
#if !defined(NDEBUG)
                if(positive>0) assert(0!=item[positive-1]);
                assert(Memory::OutOfReach::Are0(item+positive,(capacity-positive)*sizeof(T)));
#endif
            }

            //! check positive are matching bits (debug)
            inline bool areMatching(const size_t bits) const noexcept
            {
                assert(capacity>0);
                assert(IsPowerOfTwo(capacity));
                assert(positive<=capacity);
                return BitsToPositive(bits) == positive;
            }

            //! get least significant 64 bits
            inline uint64_t pull64() const noexcept { return Pull64::From(item); }

            //! Compare(this,u)
            inline SignType compareToByte(const uint8_t u) const noexcept
            {
                const T rhs = u;
                switch(positive)
                {
                    case 0: return rhs<=0 ? __Zero__ : Negative;
                    case 1: return Sign::Of(item[0],rhs);
                    default: break;
                }
                return Positive;
            }


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t    capacity; //!< current capacity
            size_t          positive; //!< current positive itmes
            T * const       item;     //!< items in little endian

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Assembly);
        };
    }

}

#endif
