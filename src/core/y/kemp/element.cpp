
#include "y/kemp/element.hpp"
#include "y/memory/allocator/archon.hpp"
#include "y/system/exception.hpp"
#include "y/ptr/auto.hpp"
#include <cerrno>
#include <cstring>

namespace Yttrium
{
    namespace Kemp
    {
        Y_SHALLOW_IMPL(ToNum64);

        const char * const Element:: CallSign              = "MPK::Element";
        const State        Element:: Inner[Sizes]          = { AsBytes, AsNum16, AsNum32, AsNum64 };
        const State        Element:: Outer[Sizes][Sizes-1] =
        {
            { AsNum16, AsNum32, AsNum64 },
            { AsBytes, AsNum32, AsNum64 },
            { AsBytes, AsNum16, AsNum64 },
            { AsBytes, AsNum16, AsNum32 }
        };


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

        Element * Element:: Zero()
        {
            return new Element(0,AsCapacity);
        }



        Element:: Element(const Element &el) :
        Object(),
        Memory::ReadOnlyBuffer(),
        state(el.state),
        bits(el.bits),
        shift( ShiftFor( el.bytes.positive)  ),
        entry( Memory::Archon::Acquire( Coerce(shift) ) ),
        bytes(entry,One<<shift,       el.bytes.positive),
        num16(entry,bytes.capacity>>1,el.num16.positive),
        num32(entry,num16.capacity>>1,el.num32.positive),
        num64(entry,num32.capacity>>1,el.num64.positive)
        {
            memcpy(entry,el.entry,bytes.capacity);
        }


#define Y_Element_Ctor_Bits()                      \
entry( Memory::Archon::Acquire( Coerce(shift) ) ), \
bytes(entry,One<<shift,       bits,AsBits),        \
num16(entry,bytes.capacity>>1,bits,AsBits),        \
num32(entry,num16.capacity>>1,bits,AsBits),        \
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

        Element:: Element(const Exp2_ &, const size_t i) :
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

        size_t Element:: getStatePositive()     const noexcept
        {
            switch(state)
            {
                case AsBytes: return bytes.positive;
                case AsNum16: return num16.positive;
                case AsNum32: return num32.positive;
                case AsNum64: break;
            }
            return num64.positive;
        }

        uint8_t Element:: u8() const noexcept
        {
            switch(state)
            {
                case AsNum16: return static_cast<uint8_t>(num16.item[0]);
                case AsNum32: return static_cast<uint8_t>(num32.item[0]);
                case AsNum64: return static_cast<uint8_t>(num64.item[0]);
                case AsBytes: break;
            }
            return bytes.item[0];
        }

        SignType Element:: compareToByte(const uint8_t u) const noexcept
        {
            switch(state)
            {
                case AsNum16: return num16.compareToByte(u);
                case AsNum32: return num32.compareToByte(u);
                case AsNum64: return num64.compareToByte(u);
                case AsBytes: break;
            }
            return bytes.compareToByte(u);
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

        Element * Element:: Shrink(Element * const el) noexcept
        {
            assert(0!=el);
            static const size_t MinBytes = Memory::Archon::MinBytes;
            AutoPtr<Element>    guard(el);
            try {
                switch(el->state)
                {
                    case AsBytes:
                        if(el->bytes.mayShrinkAbove(MinBytes)) return new Element(*el);
                        break;

                    case AsNum16:
                        if(el->num16.mayShrinkAbove(MinBytes)) return new Element(*el);
                        break;

                    case AsNum32:
                        if(el->num32.mayShrinkAbove(MinBytes)) return new Element(*el);
                        break;

                    case AsNum64:
                        if(el->num64.mayShrinkAbove(MinBytes)) return new Element(*el);
                        break;
                }
            }
            catch(...) {}
            return guard.yield();
        }

        Element * Element:: Ldz(Element * const el) noexcept
        {
            assert(0!=el);
            //AutoPtr<Element> guard(el);
            switch(el->state)
            {
                case AsBytes: el->bytes.ldz(); break;
                case AsNum16: el->num16.ldz(); break;
                case AsNum32: el->num32.ldz(); break;
                case AsNum64: el->num64.ldz(); break;
            }
            el->bits = 0;
            return Shrink(el->revise());
        }


        Element * Element:: Ld1(Element * const el) noexcept
        {
            assert(0!=el);
            //AutoPtr<Element> guard(el);
            switch(el->state)
            {
                case AsBytes: el->bytes.ld1(); break;
                case AsNum16: el->num16.ld1(); break;
                case AsNum32: el->num32.ld1(); break;
                case AsNum64: el->num64.ld1(); break;
            }
            el->bits = 1;
            return Shrink(el->revise());
        }


        Element * Element:: revise() noexcept
        {
            switch(state)
            {
                case AsBytes: assert(bytes.areMatching(bits));
                    num16.updatePositive(bits);
                    num32.updatePositive(bits);
                    num64.updatePositive(bits);
                    break;

                case AsNum16: assert(num16.areMatching(bits));
                    bytes.updatePositive(bits);
                    num32.updatePositive(bits);
                    num64.updatePositive(bits);
                    break;

                case AsNum32: assert(num32.areMatching(bits));
                    bytes.updatePositive(bits);
                    num16.updatePositive(bits);
                    num64.updatePositive(bits);
                    break;

                case AsNum64: assert(num64.areMatching(bits));
                    bytes.updatePositive(bits);
                    num16.updatePositive(bits);
                    num32.updatePositive(bits);
                    break;

            }
            return this;
        }


    }

}


