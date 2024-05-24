#include "y/apk/element.hpp"

#include "y/memory/allocator/archon.hpp"
#include "y/system/exception.hpp"
#include "y/type/utils.hpp"
#include "y/calculus/base2.hpp"
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
        state(AsByte),
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
    }

}


