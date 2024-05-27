#include "y/apk/component.hpp"
#include "y/memory/allocator/archon.hpp"
#include "y/calculus/base2.hpp"
#include "y/system/exception.hpp"
#include "y/type/utils.hpp"
#include "y/calculus/bit-count.hpp"
#include <cstring>

namespace Yttrium
{
    namespace APK
    {

        Y_SHALLOW_IMPL(FromNum64);

        const char * const Component:: CallSign = "APK::Component";
        const InnerState   Component:: State[4] = { AsBytes, AsNum16, AsNum32, AsNum64 };
        
        std::ostream & operator<<(std::ostream &os, const Component &self)
        {
            switch(self.state)
            {
                case AsBytes: os << self.bytes; break;;
                case AsNum16: os << self.num16; break;;
                case AsNum32: os << self.num32; break;;
                case AsNum64: os << self.num64; break;;
            }
            return os;
        }

        void * Component:: EntryFor(const size_t usrBytes,
                                   unsigned    &usrShift)
        {
            static const unsigned MinShift64 = iLog2<sizeof(uint64_t)>::Value;
            static const unsigned MinShiftIO = Memory::Archon::MinShift;
            static const unsigned MinShift   = Max(MinShift64,MinShiftIO);
            static const size_t   MinBytes   = size_t(1) << MinShift;

            if(usrBytes>=Base2<size_t>::MaxPowerOfTwo) throw Libc::Exception(ENOMEM, "%s(%lu bytes)", CallSign, static_cast<unsigned long>(usrBytes));

            size_t count = MinBytes;
            usrShift     = MinShift;
            while(count<usrBytes)
            {
                count <<= 1;
                ++usrShift;
            }

            return Memory::Archon::Acquire(usrShift);
        }





#define Y_APK_Component_Ctor()   \
bytes(entry,One  <<  shift),     \
num16(entry,bytes.space>>1),     \
num32(entry,num16.space>>1),     \
num64(entry,num32.space>>1)
        
        Component:: Component(const size_t usrBytes) :
        Object(),
        bits(0),
        state(AsBytes),
        shift(0),
        entry( EntryFor(usrBytes,shift) ),
        Y_APK_Component_Ctor()
        {

        }

        Component:: Component(const Component &other) :
        Object(),
        bits(other.bits),
        state(other.state),
        shift( 0 ),
        entry( EntryFor(other.bytes.count,shift) ),
        Y_APK_Component_Ctor()
        {
            assert(bytes.space>=other.bytes.count);
            memcpy(entry,other.entry,bytes.space);
            Coerce(bytes.count) = other.bytes.count;
            Coerce(num16.count) = other.num16.count;
            Coerce(num32.count) = other.num32.count;
            Coerce(num64.count) = other.num64.count;
        }


        Component:: Component(const FromNum64_ &, const uint64_t qw) :
        Object(),
        bits( BitCount::For(qw) ),
        state( AsNum64 ),
        shift(0),
        entry( EntryFor( sizeof(uint64_t), shift)),
        bytes(entry,One  <<  shift,  Y_ALIGN8(bits)/8  ),
        num16(entry,bytes.space>>1, Y_ALIGN16(bits)/16 ),
        num32(entry,num16.space>>1, Y_ALIGN32(bits)/32 ),
        num64(entry,num32.space>>1, Y_ALIGN64(bits)/64 )
        {
            *(num64.entry) = qw;
        }

        Component:: Component(const size_t numBits, Random::Bits &ran) :
        Object(),
        bits(numBits),
        state(AsBytes),
        shift(0),
        entry(  EntryFor( Y_ALIGN8(bits)/8,shift) ),
        Y_APK_Component_Ctor()
        {
            if(bits>0)
            {

                const size_t   msi = (Coerce(bytes.count) = Y_ALIGN8(bits)/8)-1; assert(bytes.count>0);
                const unsigned msn = static_cast<unsigned>(bits - (msi<<3));     assert(msn>0);

                for(size_t i=0;i<msi;++i) Coerce(bytes.entry[i]) = ran.to<uint8_t>();

                Coerce(bytes.entry[msi]) = ran.to<uint8_t>(msn);
                Coerce(num16.count)      = Y_ALIGN16(bits)/16;
                Coerce(num32.count)      = Y_ALIGN32(bits)/32;
                Coerce(num64.count)      = Y_ALIGN64(bits)/64;
            }
        }

        Component:: ~Component() noexcept
        {
            Memory::Archon::Release(entry,shift);
        }


        Component & Component:: set(const InnerState st) noexcept
        {
            switch(st)
            {
                case AsBytes:
                    switch(state)
                    {
                        case AsBytes: return *this;
                        case AsNum16: Coerce(bytes).load(num16); break;
                        case AsNum32: Coerce(bytes).load(num32); break;
                        case AsNum64: Coerce(bytes).load(num64); break;
                    }
                    Coerce(state) = st;
                    break;

                case AsNum16:
                    switch(state)
                    {
                        case AsBytes: Coerce(num16).load(bytes); break;
                        case AsNum16: return *this;
                        case AsNum32: Coerce(num16).load(num32); break;
                        case AsNum64: Coerce(num16).load(num64); break;
                    }
                    Coerce(state) = AsNum16;
                    break;

                case AsNum32:
                    switch(state)
                    {
                        case AsBytes: Coerce(num32).load(bytes); break;
                        case AsNum16: Coerce(num32).load(num16); break;
                        case AsNum32: return *this;
                        case AsNum64: Coerce(num32).load(num64); break;
                    }
                    Coerce(state) = AsNum32;
                    break;

                case AsNum64:
                    switch(state)
                    {
                        case AsBytes: Coerce(num64).load(bytes); break;
                        case AsNum16: Coerce(num64).load(num16); break;
                        case AsNum32: Coerce(num64).load(num32); break;
                        case AsNum64: return *this;
                    }
                    Coerce(state) = AsNum64;
                    break;
            }
            return *this;
        }


        void * Component:: Tuned(const InnerState st, uint64_t &qw, size_t &nc) noexcept
        {
            void * const addr = &qw;
            nc                = 0;
            if(qw>0)
            {
                switch(st)
                {
                    case AsBytes: {
                        uint8_t * const p    = (uint8_t *) addr;
                        const uint8_t   w[8] = {
                            uint8_t(qw),     uint8_t(qw>>8),  uint8_t(qw>>16), uint8_t(qw>>24),
                            uint8_t(qw>>32), uint8_t(qw>>40), uint8_t(qw>>48), uint8_t(qw>>56)
                        };
                        if( ( p[0] = w[0] ) > 0) nc=1;
                        if( ( p[1] = w[1] ) > 0) nc=2;
                        if( ( p[2] = w[2] ) > 0) nc=3;
                        if( ( p[3] = w[3] ) > 0) nc=4;
                        if( ( p[4] = w[4] ) > 0) nc=5;
                        if( ( p[5] = w[5] ) > 0) nc=6;
                        if( ( p[6] = w[6] ) > 0) nc=7;
                        if( ( p[7] = w[7] ) > 0) nc=8;
                        assert(nc>=1);
                    } break;

                    case AsNum16: {
                        uint16_t * const p    = (uint16_t *) addr;
                        const uint16_t   w[4] = { uint16_t(qw), uint16_t(qw>>16), uint16_t(qw>>32), uint16_t(qw>>48) };
                        if( ( p[0] = w[0] ) > 0) nc=1;
                        if( ( p[1] = w[1] ) > 0) nc=2;
                        if( ( p[2] = w[2] ) > 0) nc=3;
                        if( ( p[3] = w[3] ) > 0) nc=4;
                        assert(nc>=1);
                    } break;

                    case AsNum32: {
                        uint32_t * const p    = (uint32_t *) addr;
                        const uint32_t   w[2] = { uint32_t(qw), uint32_t(qw>>32) };
                        if( ( p[0] = w[0] ) > 0) nc=1;
                        if( ( p[1] = w[1] ) > 0) nc=2;
                        assert(nc>=1);
                    } break;

                    case AsNum64:
                        nc=1;
                        break;
                }
                assert(nc>=1);
            }

            return addr;
        }
    }
}
