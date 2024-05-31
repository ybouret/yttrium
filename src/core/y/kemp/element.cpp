
#include "y/kemp/element.hpp"
#include "y/memory/allocator/archon.hpp"
#include "y/system/exception.hpp"
#include <cerrno>
#include <cstring>

namespace Yttrium
{
    namespace Kemp
    {
        Y_SHALLOW_IMPL(ToNum64);

        const char * const Element:: CallSign = "MPK::Element";
        const State        Element:: Inner[Sizes] = { AsBytes, AsNum16, AsNum32, AsNum64 };
        const Ops          Element:: Proto[Kinds] =
        {
            Ops64_32,
            Ops64_16,
            Ops64_8,
            Ops32_16,
            Ops32_8,
            Ops16_8
        };


        Element:: Element(const size_t usrBytes, const AsCapacity_ &) :
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


        Element:: Element(const Element &el) :
        Object(),
        Memory::ReadOnlyBuffer(),
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


#define Y_Element_Ctor_Bits() \
entry( Memory::Archon::Acquire( Coerce(shift) ) ),\
bytes(entry,One<<shift,       bits,AsBits),\
num16(entry,bytes.capacity>>1,bits,AsBits),\
num32(entry,num16.capacity>>1,bits,AsBits),\
num64(entry,num32.capacity>>1,bits,AsBits)

        Element:: Element(const uint64_t qw, const ToNum64_ &) :
        state(AsNum64),
        bits(  BitCount::For(qw) ),
        shift( ShiftFor( sizeof(uint64_t)) ),
        Y_Element_Ctor_Bits()
        {
            num64.item[0] = qw;
        }




        Element:: Element(const size_t nbits, Random::Bits &ran) :
        state( AsBytes ),
        bits( nbits ),
        shift( ShiftFor( Bytes::BitsToPositive(bits)) ),
        Y_Element_Ctor_Bits()
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

        Element:: Element(const TwoToThe_ &, const size_t i) :
        state( AsBytes ),
        bits( i+1 ),
        shift( ShiftFor( Bytes::BitsToPositive(bits)) ),
        Y_Element_Ctor_Bits()
        {

            assert(bytes.positive>0);
            const size_t msi = bytes.positive-1;
            const size_t msb = bits - (msi<<3);  
            assert(msb>=1);
            assert(msb<=8);
            assert(bits==msi*8+msb);
            static const uint8_t one = 1;
            bytes.item[msi] = (one << (msb-1));

        }


        Element:: ~Element() noexcept
        {
            Memory::Archon::Release(entry,shift);
            state         = AsBytes;
            bits          = 0;
            Coerce(entry) = 0;
            Coerce(shift) = 0;
        }


        size_t       Element ::measure() const noexcept { return bytes.capacity; }
        const void * Element:: ro_addr() const noexcept { return entry; }
        



        std::ostream & operator<<(std::ostream &os, const Element &el)
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

        unsigned Element:: ShiftFor(const size_t usrBytes)
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


        template <> Assembly<uint8_t> & Element:: get<uint8_t>() noexcept {
            return set(AsBytes).bytes;
        }

        template <> Assembly<uint16_t> & Element:: get<uint16_t>() noexcept {
            return set(AsNum16).num16;
        }

        template <> Assembly<uint32_t> & Element:: get<uint32_t>() noexcept {
            return set(AsNum32).num32;
        }


        template <> Assembly<uint64_t> & Element:: get<uint64_t>() noexcept {
            return set(AsNum64).num64;
        }


        uint64_t Element:: u64() const noexcept
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


        Element & Element:: set(const State newState) noexcept
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

        State  Element:: TuneUp(Element &lhs, Element &rhs) noexcept
        {
            switch( Sign::Of(lhs.state,rhs.state) )
            {
                case Negative: return lhs.set(rhs.state).state;
                case Positive: return rhs.set(lhs.state).state;
                case __Zero__: break;
            }
            return lhs.state;
        }

 


    }

}


