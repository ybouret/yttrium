
#include "y/utest/run.hpp"

#include "y/calculus/bit-count.hpp"
#include "y/calculus/byte-count.hpp"

#include "y/random/park-miller.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/memory/allocator/archon.hpp"
#include "y/system/exception.hpp"
#include "y/tow/api.hpp"
#include "y/type/capacity.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/buffer/ro.hpp"
#include <cerrno>
#include <cstring>

namespace Yttrium
{
    namespace MPK
    {

        //! reworking 64-bits input into another uint64_t
        struct Pull64
        {
            static uint64_t From(const uint8_t  * const) noexcept; //!< rebuild from bytes
            static uint64_t From(const uint16_t * const) noexcept; //!< rebuild from words
            static uint64_t From(const uint32_t * const) noexcept; //!< rebuild from dwords
            static uint64_t From(const uint64_t * const) noexcept; //!< read

        };

        
        uint64_t Pull64:: From(const uint8_t * const p) noexcept
        {
            assert(0!=p);
            const uint64_t w[8] = { p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7] };
            return w[0] | (w[1]<<8) | (w[2]<<16) | (w[3]<<24) | (w[4]<<32) | (w[5]<<40) | (w[6]<<48) | (w[7]<<56);
        }


        uint64_t Pull64:: From(const uint16_t * const p) noexcept
        {
            assert(0!=p);
            const uint64_t w[4] = { p[0], p[1], p[2], p[3] };
            return w[0] | (w[1]<<16) | (w[2]<<32) | (w[3]<<48);
        }

        uint64_t Pull64:: From(const uint32_t * const p) noexcept
        {
            assert(0!=p);
            const uint64_t w[2] = { p[0], p[1] };
            return w[0] | (w[1]<<32);
        }

        uint64_t Pull64:: From(const uint64_t * const p) noexcept
        {
            assert(0!=p);
            return *p;
        }


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

            inline uint64_t pull64() const noexcept
            {
                return Pull64::From(item);
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


        class Element : public Object, public Memory::ReadOnlyBuffer
        {
        public:
            static const char * const CallSign;
            static const size_t       One = 1;
            static const State        Inner[4];

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

            explicit Element(const uint64_t qw, const ToNum64_ &) :
            state(AsNum64),
            bits(  BitCount::For(qw) ),
            shift( ShiftFor( sizeof(uint64_t)) ),
            entry( Memory::Archon::Acquire( Coerce(shift) ) ),
            bytes(entry,One<<shift,       bits,AsBits),
            num16(entry,bytes.capacity>>1,bits,AsBits),
            num32(entry,num16.capacity>>1,bits,AsBits),
            num64(entry,num32.capacity>>1,bits,AsBits)
            {
                num64.item[0] = qw;
            }

            explicit Element(const size_t nbits, Random::Bits &ran) :
            state( AsBytes ),
            bits( nbits ),
            shift( ShiftFor( Bytes::BitsToPositive(bits)) ),
            entry( Memory::Archon::Acquire( Coerce(shift) ) ),
            bytes(entry,One<<shift,       bits,AsBits),
            num16(entry,bytes.capacity>>1,bits,AsBits),
            num32(entry,num16.capacity>>1,bits,AsBits),
            num64(entry,num32.capacity>>1,bits,AsBits)
            {
                if(bits>0)
                {
                    assert(bytes.positive>0);
                    const size_t msi = bytes.positive-1;
                    const size_t msb = bits - (msi<<3);  assert(msb>0); assert(bits==msi*8+msb);
                    for(size_t i=0;i<msi;++i) bytes.item[i] = ran.to<uint8_t>();
                    bytes.item[msi] = ran.to<uint8_t>( unsigned(msb) );

                    assert(bits==bytes.updateBits());
                }
            }


            virtual ~Element() noexcept
            {
                Memory::Archon::Release(entry,shift);
                state         = AsBytes;
                bits          = 0;
                Coerce(entry) = 0;
                Coerce(shift) = 0;
            }

            friend std::ostream & operator<<(std::ostream &os, const Element &el)
            {
                switch(el.state)
                {
                    case AsBytes: os << el.bytes; break;
                    case AsNum16: os << el.num16; break;
                    case AsNum32: os << el.num32; break;
                    case AsNum64: os << el.num64; break;
                }
                return os;
            }

            virtual size_t       measure() const noexcept { return bytes.capacity; }
            virtual const void * ro_addr() const noexcept { return entry; }

            uint64_t u64() const noexcept
            {
                switch(state)
                {
                    case AsBytes: return bytes.pull64();
                    case AsNum16: return num16.pull64();
                    case AsNum32: return num32.pull64();
                    case AsNum64: break;
                }
                return num64.pull64();
            }

            Element & set(const State newState) noexcept
            {
                switch(newState)
                {
                    case AsBytes: 
                        switch(state)
                        {
                            case AsBytes: return *this;
                            case AsNum16: bytes.load(num16); break;
                            case AsNum32: bytes.load(num32); break;
                            case AsNum64: bytes.load(num64); break;
                        }
                        break;

                    case AsNum16:
                        switch(state)
                        {
                            case AsBytes: num16.load(bytes); break;
                            case AsNum16: return *this;
                            case AsNum32: num16.load(num32); break;
                            case AsNum64: num16.load(num64); break;
                        }
                        break;
                    case AsNum32:
                        switch(state)
                        {
                            case AsBytes: num32.load(bytes); break;
                            case AsNum16: num32.load(num16); break;
                            case AsNum32: return *this;
                            case AsNum64: num32.load(num64); break;
                        }
                        break;

                    case AsNum64:
                        switch(state)
                        {
                            case AsBytes: num64.load(bytes); break;
                            case AsNum16: num64.load(num16); break;
                            case AsNum32: num64.load(num32); break;
                            case AsNum64: return *this;
                        }
                        break;
                }
                assert(newState!=state);
                state = newState;
                return *this;
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
        const State        Element:: Inner[4] = { AsBytes, AsNum16, AsNum32, AsNum64 };


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

    std::cerr << "<ToNum64>" << std::endl;
    for(unsigned i=0;i<=64;++i)
    {
        const uint64_t qw = ran.to<uint64_t>(i); Y_ASSERT(i==BitCount::For(qw));
        MPK::Element   el(qw,MPK::ToNum64);
        Y_ASSERT(el.bits == i);
        Y_ASSERT(MPK::AsNum64 == el.state);
        for(unsigned k=0;k<4;++k)
        {
            for(unsigned l=0;l<4;++l)
            {
                Y_ASSERT( el.set( MPK::Element::Inner[k]).u64() == qw );
                Y_ASSERT( el.set( MPK::Element::Inner[l]).u64() == qw );
            }
        }

    }

    std::cerr << "<Random>" << std::endl;
    for(unsigned i=0;i<=1024;++i)
    {
        MPK::Element    el(i,ran);
        Vector<uint8_t> org(el.bytes.capacity,AsCapacity);
        for(size_t j=0;j<el.bytes.capacity;++j) {
            org << el.bytes.item[j];
        }
        Y_ASSERT(org.size()==el.measure());

        for(unsigned k=0;k<4;++k)
        {
            for(unsigned l=0;l<4;++l)
            {
                (void) el.set( MPK::Element::Inner[k]);
                (void) el.set( MPK::Element::Inner[l]);
                el.set(MPK::AsBytes);
                Y_ASSERT( 0 == memcmp( &org[1], el.ro_addr(), el.measure() ) );
            }
        }

    }

}
Y_UDONE()

