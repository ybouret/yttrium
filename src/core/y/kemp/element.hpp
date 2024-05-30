
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

        typedef Assembly<uint8_t>  Bytes; //!< alias
        typedef Assembly<uint16_t> Num16; //!< alias
        typedef Assembly<uint32_t> Num32; //!< alias
        typedef Assembly<uint64_t> Num64; //!< alias

        //! internal state representation
        enum  State
        {
            AsBytes, //!< use Bytes
            AsNum16, //!< use Num16
            AsNum32, //!< use Num32
            AsNum64  //!< use Num64
        };

        //! alias
        Y_SHALLOW_DECL(ToNum64);

        //______________________________________________________________________
        //
        //
        //
        //! Element with mutltiple synchronous assemblies
        //
        //
        //______________________________________________________________________
        class Element : public Object, public Memory::ReadOnlyBuffer
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Kemp::Element"
            static const size_t       One = 1;  //!< alias
            static const State        Inner[4]; //!< aliases

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Element(const size_t, const AsCapacity_ &); //!< setup with capacity
            explicit Element(const Element &);                   //!< full copy
            explicit Element(const uint64_t, const ToNum64_ &);  //!< setup from qword
            explicit Element(const size_t, Random::Bits &);      //!< setup to random bits
            virtual ~Element() noexcept;                         //!< cleanup
            Y_OSTREAM_PROTO(Element);                            //!< display

            //__________________________________________________________________
            //
            //
            // [Memory::ReadOnlyBuffer]
            //
            //__________________________________________________________________
            virtual size_t       measure() const noexcept;
            virtual const void * ro_addr() const noexcept;

            //__________________________________________________________________
            //
            //
            // Manipulations methods
            //
            //__________________________________________________________________  
            uint64_t     u64()                  const noexcept; //!< least significant 64 bits
            Element &    set(const State)             noexcept; //!< ensure state
            static State TuneUp(Element &, Element &) noexcept; //!< tune to largest integral

            //__________________________________________________________________
            //
            //
            // Comparisons
            //
            //__________________________________________________________________
            static SignType Compare(Element &, Element&)          noexcept; //!< compare with TuneUp
            static SignType Compare(const Element &, uint64_t qw) noexcept; //!< compare using element's state
            static SignType Compare(uint64_t qw, const Element &) noexcept; //!< compare using element's state


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
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

