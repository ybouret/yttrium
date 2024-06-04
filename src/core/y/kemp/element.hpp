
//! \file

#ifndef Y_Kemp_Element_Included
#define Y_Kemp_Element_Included 1

#include "y/kemp/types.hpp"
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
        //! named logical operators
        //
        //______________________________________________________________________
        enum Logical
        {
            AND, //! &
            OR,  //! |
            XOR  //! ^
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
            static const char * const CallSign;                         //!< "Kemp::Element"
            static const size_t       One = 1;                          //!< alias
            static const unsigned     Sizes = 4;                        //!< 8,16,32 and 64 bits
            static const State        Inner[Sizes];                     //!< aliases
            static const State        Outer[Sizes][Sizes-1];            //!< aliases
            static const unsigned     Kinds = ( Sizes *(Sizes-1 )) / 2; //!< combinations
            static const Ops          Proto[Kinds];                     //!< aliases

            //__________________________________________________________________
            //
            //
            // Procedures
            //
            //__________________________________________________________________
            typedef Element * (*BinaryProc)(Element &lhs, Element &rhs);               //!< binary procedure
            typedef Element * (*BinaryProcEx)(Element &lhs, Element &rhs, uint64_t &); //!< binary procedure with timing
            typedef Element * (*BinL64Proc)(uint64_t lhs, Element &rhs);               //!< binary procedure with unsigned lhs
            typedef Element * (*BinR64Proc)(Element &lhs, uint64_t rhs);               //!< binary procedure with unsigned rhs

            //__________________________________________________________________
            //
            //! set of Binary procedures
            //__________________________________________________________________
            struct BinaryAPI
            {
                BinaryProc   const Result; //!< produce result (lhs,rhs)
                BinaryProcEx const ResTMX; //!< produce result (lhs,rhs,tmx)
                BinL64Proc   const ResL64; //!< produce result (qw,rhs)
                BinR64Proc   const ResR64; //!< produce result (rhs,qw)

                inline Element * operator()(Element &lhs, Element &rhs)                const { return Result(lhs,rhs);     } //!< select result
                inline Element * operator()(Element &lhs, Element &rhs, uint64_t &tmx) const { return ResTMX(lhs,rhs,tmx); } //!< select result
                inline Element * operator()(uint64_t lhs, Element &rhs)                const { return ResL64(lhs,rhs);     } //!< select result
                inline Element * operator()(Element &lhs, uint64_t rhs)                const { return ResR64(lhs,rhs);     } //!< select result

            };

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
            explicit Element(const TwoToThe_ &, const size_t i); //!< 2^i
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
            Assembly<T> &   get()                        noexcept; //!< return prepared assembly
            uint64_t        u64()                  const noexcept; //!< least significant 64 bits
            Element &       set(const State)             noexcept; //!< ensure state
            static State    TuneUp(Element &, Element &) noexcept; //!< tune to largest integral
            static Element *Shrink(Element *)            noexcept; //!< try to reduce capacity
            Element *       revise()                     noexcept; //!< update positive in other states

            //__________________________________________________________________
            //
            //
            // Bits
            //
            //__________________________________________________________________
            Element & shr() noexcept;           //!< IN PLACE shift right 1 bit, noexcept (a.k.a fast divide by two)
            Element * shr(const size_t nbit);   //!< shift right, new element
            Element * shl(const size_t nbit);   //!< shift left,  new element

            //__________________________________________________________________
            //
            //
            // Comparisons
            //
            //__________________________________________________________________
            static SignType Compare(Element &, Element&)          noexcept; //!< compare with TuneUp
            static SignType Compare(const Element &, uint64_t qw) noexcept; //!< compare using element's state
            static SignType Compare(uint64_t qw, const Element &) noexcept; //!< compare using element's state
            static Element *Bitwise(Logical op, Element &, Element &);      //!< produce a bitwise op


            //__________________________________________________________________
            //
            //
            // Ops
            //
            //__________________________________________________________________
            static const BinaryAPI Add[Kinds]; //!< all possible additions
            static const BinaryAPI Sub[Kinds]; //!< all possible subtractions
            static const BinaryAPI Mul[Kinds]; //!< all possible long multiplications
            static const BinaryAPI MulFFT;     //!< FFT multiplication API

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

