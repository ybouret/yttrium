
#include "y/utest/run.hpp"

#include "y/calculus/bit-count.hpp"
#include "y/calculus/byte-count.hpp"

#include "y/random/park-miller.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/memory/allocator/archon.hpp"
#include "y/system/exception.hpp"
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

        //! interface for assebmly
        class Metrics
        {
        public:
            virtual ~Metrics() noexcept { Coerce(positive)=0; Coerce(capacity)=0; }

        protected:
            explicit Metrics(const size_t num,
                             const size_t max) noexcept :
            positive(num),
            capacity(max)
            {
                assert(positive<=capacity);
                assert(capacity>0);
            }

        public:
            virtual size_t updateBits() noexcept = 0;

        public:
            const size_t positive;
            const size_t capacity;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Metrics);
        };


        template <typename T>
        class Assembly : public Metrics
        {
        public:
            typedef T             WordType;
            static const unsigned WordSize     = sizeof(WordType);
            static const unsigned WordBits     = WordSize << 3;
            static const unsigned Log2WordBits = iLog2<WordBits>::Value;

            inline Assembly(uint64_t &qw) noexcept :
            Metrics( Push64::To( (T*)&qw, qw),sizeof(uint64_t) / WordSize ),
            entry( (T*) & qw )
            {
            }

            inline Assembly(const void * const data,
                            const size_t       capa,
                            const size_t       npos=0) noexcept :
            Metrics(npos,capa),
            entry( static_cast<const T *>(data) )
            {
                assert(0!=entry);
            }



            inline friend std::ostream & operator<<(std::ostream &os, const Assembly &self)
            {
                os << '[' << std::setw(3) << self.positive << '/' << std::setw(3) << self.capacity <<  ']' << '@' << (const void *) self.entry;
                Hexadecimal::Display(os << '=',self.entry,self.positive);
                return os;
            }

            inline ~Assembly() noexcept
            {
            }

            //! checking number of bits with most significant word
            virtual size_t updateBits() noexcept
            {
                while(positive>0)
                {
                    const size_t msi = positive-1;
                    const T      top = entry[msi];
                    if(top>0)
                    {
                        return (msi<<Log2WordBits) + BitCount::For(top);
                    }
                    Coerce(positive) = msi;
                }
                return 0;
            }


            const T * const entry;

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


        class Element : public Object
        {
        public:
            static const char * const CallSign;
            static const size_t       One = 1;

            explicit Element(const size_t usrBytes) :
            state( AsBytes ),
            bits( 0 ),
            shift( ShiftFor( usrBytes ) ),
            entry( Memory::Archon::Acquire( Coerce(shift) ) ),
            count( One << shift ),
            wksp()
            {
                bytes = new (wksp) Bytes(entry,count,0);
            }

            ~Element() noexcept
            {
                Memory::Archon::Release(entry,shift);
                Coerce(bits)  = 0;
                Coerce(shift) = 0;
                Coerce(entry) = 0;
                Coerce(count) = 0;
            }

            const State    state;
            const size_t   bits;
            const unsigned shift;
            void  * const  entry;
            const size_t   count;
            union {
                const Bytes *bytes;
                const Num16 *num16;
                const Num32 *num32;
                const Num64 *num64;
            };
            void *wksp[Y_WORDS_FOR(Bytes)];

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

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Element);
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



    MPK::Element el(0);

}
Y_UDONE()

