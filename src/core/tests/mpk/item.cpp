
#include "y/utest/run.hpp"

#include "y/calculus/bit-count.hpp"
#include "y/calculus/byte-count.hpp"

#include "y/random/park-miller.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/memory/allocator/archon.hpp"
#include "y/system/exception.hpp"
#include "y/tow/api.hpp"
#include "y/type/capacity.hpp"

#include <cerrno>

namespace Yttrium
{
    namespace MPK
    {

        //! reworking 64-bits input into another uint64_t
        struct Push64
        {
            static size_t To(uint8_t  * const p, const uint64_t qw) noexcept; //!< split, return bytes
            static size_t To(uint16_t * const p, const uint64_t qw) noexcept; //!< split, return words
            static size_t To(uint32_t * const p, const uint64_t qw) noexcept; //!< split, return dwords
            static size_t To(uint64_t * const p, const uint64_t qw) noexcept; //!< do nothing, return qw>0
        };

        size_t Push64:: To(uint8_t  * const p, const uint64_t qw) noexcept
        {
            assert(0!=p);
            size_t count = 0;
            if( (p[0] = static_cast<uint8_t>(qw)    )>0 ) count=1;
            if( (p[1] = static_cast<uint8_t>(qw>>8) )>0 ) count=2;
            if( (p[2] = static_cast<uint8_t>(qw>>16))>0 ) count=3;
            if( (p[3] = static_cast<uint8_t>(qw>>24))>0 ) count=4;
            if( (p[4] = static_cast<uint8_t>(qw>>32))>0 ) count=5;
            if( (p[5] = static_cast<uint8_t>(qw>>40))>0 ) count=6;
            if( (p[6] = static_cast<uint8_t>(qw>>48))>0 ) count=7;
            if( (p[7] = static_cast<uint8_t>(qw>>56))>0 ) count=8;
            return count;
        }

        size_t Push64:: To(uint16_t  *const p, const uint64_t qw) noexcept
        {
            assert(0!=p);
            size_t count = 0;
            if( (p[0] = static_cast<uint16_t>(qw)     ) >0 ) count=1;
            if( (p[1] = static_cast<uint16_t>(qw>>16) ) >0 ) count=2;
            if( (p[2] = static_cast<uint16_t>(qw>>32) ) >0 ) count=3;
            if( (p[3] = static_cast<uint16_t>(qw>>48) ) >0 ) count=4;
            return count;
        }

        size_t Push64:: To(uint32_t  * const p,  const uint64_t qw) noexcept
        {
            assert(0!=p);
            size_t count = 0;
            if( (p[0] = static_cast<uint32_t>(qw)     ) >0 ) count=1;
            if( (p[1] = static_cast<uint32_t>(qw>>32) ) >0 ) count=2;
            return count;
        }

        size_t Push64:: To(uint64_t  * const,  const uint64_t qw) noexcept
        {
            return qw > 0 ? 1 : 0;
        }

        Y_SHALLOW_DECL(AsBits);

        Y_SHALLOW_IMPL(AsBits);



        //! the number of positive items is handled by Element
        template <typename T>
        class Assembly
        {
        public:
            typedef T             WordType;
            static const unsigned WordSize     = sizeof(WordType);
            static const unsigned WordBits     = WordSize << 3;
            static const unsigned Log2WordBits = iLog2<WordBits>::Value;

            static inline size_t BitsToPositive(const size_t bits) noexcept
            {
                return (Y_ALIGN_LN2(Log2WordBits,bits)) >> Log2WordBits;
            }

            //! build from a PERSISTENT QWORD
            inline Assembly(uint64_t &qw) noexcept :
            capacity( sizeof(uint64_t) / WordSize ),
            positive( Push64::To( (T*)&qw, qw)    ),
            item( (T*) & qw )
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

            //! build from PERSISTENT user data
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


            inline friend std::ostream & operator<<(std::ostream &os, const Assembly &self)
            {
                os << '[' << std::setw(3) << self.positive << '/' << std::setw(3) << self.capacity <<  ']' << '@' << (const void *) self.item;
                Hexadecimal::Display(os << '=',self.item,self.positive);
                return os;
            }

            inline ~Assembly() noexcept
            {
                Coerce(capacity) = 0;
                positive         = 0;
                Coerce(item)     = 0;
            }


            //! transmogrify from another assembly
            template <typename U> inline
            void load(const Assembly<U> &source)
            {
                // TARGET = T, SOURCE = U
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
                    {
                        return (msi<<Log2WordBits) + BitCount::For(top);
                    }
                    positive = msi;
                }
                assert(0==positive);
                return 0;
            }

            const size_t    capacity;
            size_t          positive;
            T * const       item;

        private:
            Y_DISABLE_ASSIGN(Assembly);
        };

        typedef Assembly<uint8_t>  Bytes;
        typedef Assembly<uint16_t> Num16;
        typedef Assembly<uint32_t> Num32;
        typedef Assembly<uint64_t> Num64;

        enum  State
        {
            AsBytes,
            AsNum16,
            AsNum32,
            AsNum64
        };

        Y_SHALLOW_DECL(ToNum64);

        Y_SHALLOW_IMPL(ToNum64);


        class Element : public Object
        {
        public:
            static const char * const CallSign;
            static const size_t       One = 1;

            explicit Element(const size_t usrBytes, const AsCapacity_ &) :
            state( AsBytes ),
            bits( 0 ),
            shift( ShiftFor( usrBytes ) ),
            entry( Memory::Archon::Acquire( Coerce(shift) ) ),
            bytes(entry,One<<shift,       0),
            num16(entry,bytes.capacity>>1,0),
            num32(entry,num16.capacity>>1,0),
            num64(entry,num32.capacity>>1,0)
            {
            }

            explicit Element(const Element &el) :
            state(el.state),
            bits(el.bits),
            shift( ShiftFor(el.bytes.positive) ),
            entry( Memory::Archon::Acquire( Coerce(shift) ) ),
            bytes(entry,One<<shift,el.bytes.positive),
            num16(entry,bytes.capacity>>1,el.num16.positive),
            num32(entry,num16.capacity>>1,el.num32.positive),
            num64(entry,num32.capacity>>1,el.num64.positive)
            {
                memcpy(entry,el.entry,bytes.capacity);
            }

            explicit Element(const ToNum64_ &, const uint64_t qw) :
            state(AsNum64),
            bits( BitCount::For(qw) ),
            shift( ShiftFor( sizeof(uint64_t)) ),
            entry( Memory::Archon::Acquire( Coerce(shift) ) ),
            bytes(entry,One<<shift,bits,AsBits),
            num16(entry,bytes.capacity>>1,bits,AsBits),
            num32(entry,num16.capacity>>1,bits,AsBits),
            num64(entry,num32.capacity>>1,bits,AsBits)
            {
                num64.item[0] = qw;
            }

            virtual ~Element() noexcept
            {
                Memory::Archon::Release(entry,shift);
                state         = AsBytes;
                bits          = 0;
                Coerce(entry) = 0;
                Coerce(shift) = 0;
            }

            State          state; //!< current state
            size_t         bits;  //!< current number of bits
        private:
            const unsigned shift; //!< 2^shift = allocated bytes
            void  * const  entry; //!< workspace
        public:
            Bytes          bytes; //!< as bytes and primary metrics
            Num16          num16; //!< as num16
            Num32          num32; //!< as num32
            Num64          num64; //!< as num64



        private:
            Y_DISABLE_ASSIGN(Element);
            static inline unsigned ShiftFor(const size_t usrBytes)
            {
                static const unsigned MinShift = Memory::Archon::MinShift;
                if( usrBytes > Base2<size_t>::MaxPowerOfTwo ) throw Libc::Exception(EINVAL,"%s Overflow", CallSign);
                unsigned s = MinShift;
                size_t   n = One << s;
                while(n<usrBytes)
                {
                    n <<= 1;
                    ++s;
                }
                return s;
            }
        };

        const char * const Element:: CallSign = "MPK::Element";



    }
}

using namespace Yttrium;

Y_UTEST(mpk_item)
{
    Random::ParkMiller ran;
    for(unsigned i=0;i<=64;++i)
    {
        const uint64_t qw = ran.to<uint64_t>(i); Y_ASSERT(BitCount::For(qw)==i);

        { uint64_t temp = qw; const MPK::Assembly<uint8_t>  a(temp); std::cerr << a << std::endl; }
        { uint64_t temp = qw; const MPK::Assembly<uint16_t> a(temp); std::cerr << a << std::endl; }
        { uint64_t temp = qw; const MPK::Assembly<uint32_t> a(temp); std::cerr << a << std::endl; }
        { uint64_t temp = qw; const MPK::Assembly<uint64_t> a(temp); std::cerr << a << std::endl; }
    }

    Y_SIZEOF(MPK::Bytes);
    Y_SIZEOF(MPK::Num16);
    Y_SIZEOF(MPK::Num32);
    Y_SIZEOF(MPK::Num64);
    Y_SIZEOF(MPK::Element);



    MPK::Element el(0,AsCapacity);
    MPK::Element el2(el);

    for(size_t bits=0;bits<=80;++bits)
    {
        std::cerr << std::setw(3) << bits;
        std::cerr << " : " <<  std::setw(3) << MPK::Assembly<uint8_t>:: BitsToPositive(bits);
        std::cerr << " : " <<  std::setw(3) << MPK::Assembly<uint16_t>::BitsToPositive(bits);
        std::cerr << " : " <<  std::setw(3) << MPK::Assembly<uint32_t>::BitsToPositive(bits);
        std::cerr << " : " <<  std::setw(3) << MPK::Assembly<uint64_t>::BitsToPositive(bits);
        std::cerr << std::endl;
    }

    

}
Y_UDONE()

