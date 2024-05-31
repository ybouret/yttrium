
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
        //______________________________________________________________________
        //
        //
        //
        // global definitions
        //
        //
        //______________________________________________________________________

        typedef Assembly<uint8_t>  Bytes; //!< alias
        typedef Assembly<uint16_t> Num16; //!< alias
        typedef Assembly<uint32_t> Num32; //!< alias
        typedef Assembly<uint64_t> Num64; //!< alias

        //______________________________________________________________________
        //
        //
        //! internal state representation
        //
        //______________________________________________________________________
        enum  State
        {
            AsBytes, //!< use Bytes
            AsNum16, //!< use Num16
            AsNum32, //!< use Num32
            AsNum64  //!< use Num64
        };

        //______________________________________________________________________
        //
        //
        //! internal operations interface
        //
        //______________________________________________________________________
        enum Ops
        {
            // 64 bits core
            Ops64_32, //!< CORE=uint64_t, WORD=uint32_t
            Ops64_16, //!< CORE=uint64_t, WORD=uint16_t
            Ops64_8,  //!< CORE=uint64_t, WORD=uint8_t
            Ops32_16, //!< CORE=uint32_t, WORD=uint16_t
            Ops32_8,  //!< CORE=uint32_t, WORD=uint8_t
            Ops16_8   //!< CORE=uint16_t, WORD=uint8_t
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
            static const char * const CallSign;     //!< "Kemp::Element"
            static const size_t       One = 1;      //!< alias
            static const unsigned     Sizes = 4;    //!< 8,16,32 and 64 bits
            static const State        Inner[Sizes]; //!< aliases
            static const unsigned     Kinds = ( Sizes *(Sizes-1 )) / 2; //!< combinations
            static const Ops          Proto[Kinds];                     //!< aliases

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
            template <typename T>
            Assembly<T> &get()                        noexcept; //!< return prepared assembly
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
            // Addition
            //
            //__________________________________________________________________
            typedef Element * (*BinaryProc)(Element &lhs, Element &rhs);               //!< binary procedure
            typedef Element * (*BinaryProcEx)(Element &lhs, Element &rhs, uint64_t &); //!< binary procedure with timing
            static  const BinaryProc   Add[Kinds];   //!< additions
            static  const BinaryProcEx AddEx[Kinds]; //!< additions with timing


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

        //______________________________________________________________________
        //
        //
        //! implementing CODE<CORE,WORD>
        //
        //______________________________________________________________________
#define Y_Kemp_Ops(CODE)  \
CODE<uint64_t,uint32_t>,  \
CODE<uint64_t,uint16_t>,  \
CODE<uint64_t,uint8_t>,   \
CODE<uint32_t,uint16_t>,  \
CODE<uint32_t,uint8_t>,   \
CODE<uint16_t,uint8_t>    \


    }
}



#endif

