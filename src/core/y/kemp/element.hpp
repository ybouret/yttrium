
//! \file

#ifndef Y_Kemp_Element_Included
#define Y_Kemp_Element_Included 1

#include "y/kemp/element/assembly.hpp"
#include "y/object.hpp"
#include "y/memory/buffer/ro.hpp"
#include "y/type/capacity.hpp"
#include "y/random/bits.hpp"
#include "y/type/signs.hpp"

namespace Yttrium
{
    namespace Kemp
    {

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


        class Element : public Object, public Memory::ReadOnlyBuffer
        {
        public:
            static const char * const CallSign;
            static const size_t       One = 1;
            static const State        Inner[4];

            explicit Element(const size_t, const AsCapacity_ &);
            explicit Element(const Element &);
            explicit Element(const uint64_t, const ToNum64_ &);
            explicit Element(const size_t, Random::Bits &);
            virtual ~Element() noexcept;
            Y_OSTREAM_PROTO(Element);

            virtual size_t       measure() const noexcept { return bytes.capacity; }
            virtual const void * ro_addr() const noexcept { return entry; }

            uint64_t  u64() const noexcept;
            Element & set(const State newState) noexcept;
            State   & TuneUp(Element &, Element &) noexcept;


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
            static   unsigned ShiftFor(const size_t usrBytes);
        };



    }
}



#endif

