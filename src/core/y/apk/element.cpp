#include "y/apk/element.hpp"

#include "y/memory/allocator/archon.hpp"
#include "y/system/exception.hpp"
#include "y/type/utils.hpp"
#include "y/calculus/base2.hpp"
#include "y/calculus/bit-count.hpp"
#include "y/check/static.hpp"
#include "y/random/bits.hpp"
#include <cstring>


namespace Yttrium
{
    namespace APK
    {

#define Y_APK_Element_Ctor_Epilog(COUNT)         \
maxBytes( MaxBytesFor(COUNT,Coerce(shift) ) ),   \
entry( Memory::Archon::Acquire(Coerce(shift) ) ),\
maxNum16(maxBytes/sizeof(uint16_t)),             \
maxNum32(maxBytes/sizeof(uint32_t)),             \
maxNum64(maxBytes/sizeof(uint64_t))

        Element:: Element(const size_t usrBytes) :
        Object(),
        bits(0),
        state(AsBytes),
        bytes(0),
        num16(0),
        num32(0),
        num64(0),
        shift(0),
        Y_APK_Element_Ctor_Epilog(usrBytes)
        {
        }


        Element:: Element(const Element &el) :
        Object(),
        bits(el.bits),
        state(el.state),
        bytes(el.bytes),
        num16(el.num16),
        num32(el.num32),
        num64(el.num64),
        shift(0),
        Y_APK_Element_Ctor_Epilog(bytes)
        {
            switch(state)
            {
                case AsBytes: memcpy(entry,el.entry,bytes);    break;
                case AsNum16: memcpy(entry,el.entry,num16<<1); break;
                case AsNum32: memcpy(entry,el.entry,num32<<2); break;
                case AsNum64: memcpy(entry,el.entry,num64<<3); break;
            }

        }

        Element:: Element(const size_t nbits, Random::Bits &ran) :
        Object(),
        bits(nbits),
        state(AsBytes),
        bytes( Y_ALIGN8(bits)  / 8  ),
        num16( Y_ALIGN16(bits) / 16 ),
        num32( Y_ALIGN32(bits) / 32 ),
        num64( Y_ALIGN64(bits) / 64 ),
        shift(0),
        Y_APK_Element_Ctor_Epilog(bytes)
        {
            std::cerr << "bits=" << bits << " => bytes=" << bytes << std::endl;

            if(bytes>0)
            {
                uint8_t * const byte = static_cast<uint8_t*>(entry); assert(0!=byte);
                const size_t    msb  = bytes-1;
                const unsigned  msn  = unsigned(bits - (msb<<3));
                assert(8*msb+msn==bits);
                for(size_t i=0;i<msb;++i)
                    byte[i] = ran.to<uint8_t>();
                byte[msb] =   ran.to<uint8_t>(msn);
            }
        }

        Element::~Element() noexcept
        {
            Memory::Archon::Release(entry,shift);
        }

        size_t  Element:: MaxBytesFor(const size_t usrBytes, unsigned &shift)
        {
            static const unsigned MinShift64 = iLog2<sizeof(uint64_t)>::Value;
            static const unsigned MinShiftIO = Memory::Archon::MinShift;
            static const unsigned MinShift   = Max(MinShift64,MinShiftIO);
            static const size_t   MinBytes   = size_t(1) << MinShift;

            if(usrBytes>=Base2<size_t>::MaxPowerOfTwo) throw Libc::Exception(ENOMEM, "APK::Element");

            size_t count = MinBytes;
            shift        = MinShift;
            while(count<usrBytes)
            {
                count <<= 1;
                ++shift;
            }
            return count;
        }

        void Element:: ldz() noexcept
        {
            memset(entry,0,maxBytes);
            Coerce(bits)  = 0;
            Coerce(bytes) = 0;
            Coerce(num16) = 0;
            Coerce(num32) = 0;
            Coerce(num64) = 0;
            Coerce(state) = AsBytes;
        }


        void Element:: ld(const uint64_t qw) noexcept
        {
            assert(maxBytes>=sizeof(qw));
            if(qw<=0) return;
            *(uint64_t *)entry = qw;
            Coerce(state) = AsNum64;
            Coerce(bits)  = BitCount::For(qw);
            Coerce(bytes) = Y_ALIGN8(bits)  / 8;
            Coerce(num16) = Y_ALIGN16(bits) / 16;
            Coerce(num32) = Y_ALIGN32(bits) / 32;
            Coerce(num64) = Y_ALIGN64(bits) / 64;
        }

    }

}

#include "y/text/hexadecimal.hpp"

namespace Yttrium
{
    namespace APK
    {
        template <typename T> static inline
        void ShowElement(std::ostream &os, const void *entry, const size_t count)
        {
            assert(Good(entry,count));
            (void)  Core::Display(os,static_cast<const T*>(entry),count,Hexadecimal::From<T>);
        }

        std::ostream & Element:: show(std::ostream &os) const
        {
            switch(state)
            {
                case AsBytes: ShowElement<uint8_t>(os,entry,bytes);  break;
                case AsNum16: ShowElement<uint16_t>(os,entry,num16); break;
                case AsNum32: ShowElement<uint32_t>(os,entry,num32); break;
                case AsNum64: ShowElement<uint64_t>(os,entry,num64); break;
            }
            return os;
        }



    }

}
