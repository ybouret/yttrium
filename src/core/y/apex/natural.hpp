
//! \file

#ifndef Y_Apex_Natural_Included
#define Y_Apex_Natural_Included 1

#include "y/apex/number.hpp"
#include "y/apex/block/ptr.hpp"
#include "y/apex/block/mtx.hpp"
#include "y/ostream-proto.hpp"
#include "y/random/bits.hpp"
#include "y/type/signs.hpp"
#include "y/string.hpp"

namespace Yttrium
{
    class InputStream;
    
    namespace Apex
    {
        class Block;

        Y_SHALLOW_DECL(AsBlock); //!< alias
        Y_SHALLOW_DECL(Exp2);    //!< alias
        
        //______________________________________________________________________
        //
        //
        //! list of  Ops
        //
        //______________________________________________________________________
#define Y_Apex_Ops_List \
/**/           Ops8_1,  \
/**/           Ops8_2,  \
/**/           Ops8_4,  \
/**/           Ops4_1,  \
/**/           Ops4_2,  \
/**/           Ops2_1

        //______________________________________________________________________
        //
        //
        //! Named adding operation style
        //
        //______________________________________________________________________
        enum Ops { Y_Apex_Ops_List };


        //! helper to implement comparisons
#define Y_Apex_Natural_Op(OP,LHS,RHS,MATCHES,RESULT)             \
/**/ inline friend                                               \
/**/ bool  operator OP (const LHS lhs, const RHS rhs) noexcept { \
/**/   return RESULT MATCHES Natural::Compare(lhs,rhs);          \
/**/ }

        //! helper to implement comparisons
#define Y_Apex_Natural_Ops(OP,MATCHES,RESULT)               \
Y_Apex_Natural_Op(OP,Natural &, Natural &, MATCHES, RESULT) \
Y_Apex_Natural_Op(OP,Natural &, natural_t, MATCHES, RESULT) \
Y_Apex_Natural_Op(OP,natural_t, Natural &, MATCHES, RESULT) \


        //______________________________________________________________________
        //
        //
        //
        //! Natural
        //
        //
        //______________________________________________________________________
        class Natural : public Number
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< Apex::Natural
            static const unsigned     NumOps = (JigAPI::Plans * (JigAPI::Plans-1) ) >> 1; //!< card(AddOps)
            static const Ops          OpsTable[NumOps];  //!< aliases
            static const char * const OpsLabel[NumOps];  //!< named
            static Ops                AddOps;            //!< default Add/Sub Ops
            static Ops                MulOps;            //!< default Mul     Ops

            //__________________________________________________________________
            //
            //! scoped lock
            //__________________________________________________________________
            class AutoLock {
            public:
                AutoLock(const Natural &) noexcept; //!< lock mutable mutex
                ~AutoLock()               noexcept; //!< unlock
            private:
                Y_DISABLE_COPY_AND_ASSIGN(AutoLock);
                const Natural &host;
            };

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Natural(const natural_t);                       //!< setup as natural
            Natural(Random::Bits &, const size_t);          //!< setup with given random bits
            virtual ~Natural() noexcept;                    //!< cleanup
            Natural(const Natural &);                       //!< duplicate
            Natural & operator=(const Natural &);           //!< assign
            Natural & operator=(const natural_t)  noexcept; //!< assign natural
            Y_OSTREAM_PROTO(Natural);                       //!< output

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual size_t       serialize(OutputStream &) const; //!< [Serializable] bytes
            virtual const char * callSign()       const noexcept; //!< [Identifiable] CallSign
            static  Natural      Read(InputStream &);             //!< read from serialized

            //__________________________________________________________________
            //
            //
            // Access
            //
            //__________________________________________________________________
            Lockable & operator*()  const noexcept; //!< access internal mutex
            Lockable * operator->() const noexcept; //!< access internal mutex
            uint64_t      lsw()     const noexcept; //!< least significant 64bits word
            Block &       _block()        noexcept; //!< internal block
            const Block & _block()  const noexcept; //!< internal const block
            Natural &     xch(Natural &)  noexcept; //!< no-throw exchange
            size_t        bits()    const noexcept; //!< current bits

            //__________________________________________________________________
            //
            //
            // Comparisons
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            // Equality
            //__________________________________________________________________
            friend bool operator==(const Natural & lhs, const Natural & rhs) noexcept; //!< lhs == rhs
            friend bool operator==(const Natural & lhs, const natural_t rhs) noexcept; //!< lhs == rhs
            friend bool operator==(const natural_t lhs, const Natural & rhs) noexcept; //!< lhs == rhs

            //__________________________________________________________________
            //
            // Different
            //__________________________________________________________________
            friend bool operator!=(const Natural & lhs, const Natural & rhs) noexcept; //!< lhs != rhs
            friend bool operator!=(const Natural & lhs, const natural_t rhs) noexcept; //!< lhs != rhs
            friend bool operator!=(const natural_t lhs, const Natural & rhs) noexcept; //!< lhs != rhs

            //__________________________________________________________________
            //
            // Comparisons
            //__________________________________________________________________
            static SignType Compare(const Natural & lhs, const Natural & rhs) noexcept; //!< Sign::Of(lhs,rhs)
            static SignType Compare(const Natural & lhs, const natural_t rhs) noexcept; //!< Sign::Of(lhs,rhs)
            static SignType Compare(const natural_t lhs, const Natural & rhs) noexcept; //!< Sign::Of(lhs,rhs)

#ifndef DOXYGEN_SHOULD_SKIP_THIS
            Y_Apex_Natural_Ops(<, ==,Negative)
            Y_Apex_Natural_Ops(>, ==,Positive)
            Y_Apex_Natural_Ops(<=,!=,Positive)
            Y_Apex_Natural_Ops(>=,!=,Negative)
#endif
            //__________________________________________________________________
            //
            //
            // Additions
            //
            //__________________________________________________________________

            //! add with optional timing
            static Block *Add(Block &lhs, Block &rhs, const Ops addOps, uint64_t * const ell);

            friend Natural operator +  (const Natural & lhs, const Natural & rhs); //!< lhs+rhs
            friend Natural operator +  (const Natural & lhs, const natural_t rhs); //!< lhs+rhs
            friend Natural operator +  (const natural_t lhs, const Natural & rhs); //!< lhs+rhs
            Natural        operator +  () const;               //!< unary +
            Natural &      operator += (const Natural & rhs);  //!< += rhs
            Natural &      operator += (const natural_t rhs);  //!< += rhs
            Natural &      operator ++ ();                     //!< ++this
            Natural        operator ++ (int);                  //!< this++
            void           incr();                             //!< +=1

            //__________________________________________________________________
            //
            //
            // Subtractions
            //
            //__________________________________________________________________
            friend Natural operator -  (const Natural & lhs, const Natural & rhs); //!< lhs-rhs
            friend Natural operator -  (const Natural & lhs, const natural_t rhs); //!< lhs-rhs
            friend Natural operator -  (const natural_t lhs, const Natural & rhs); //!< lhs-rhs
            Natural &      operator -= (const Natural & rhs); //!< -= rhs
            Natural &      operator -= (const natural_t rhs); //!< -= rhs
            Natural &      operator -- ();                    //!< --this
            Natural        operator -- (int);                 //!< this--
            void           decr();                            //!< -=1

            //__________________________________________________________________
            //
            //
            // Multiplication
            //
            //__________________________________________________________________

            //! multiplication with optional timing
            static Block *Mul(Block &lhs, Block &rhs, const Ops mulOps, uint64_t * const ell);

            friend Natural operator *  (const Natural & lhs, const Natural & rhs); //!< lhs*rhs
            friend Natural operator *  (const Natural & lhs, const natural_t rhs); //!< lhs*rhs
            friend Natural operator *  (const natural_t lhs, const Natural & rhs); //!< lhs*rhs
            Natural &      operator *= (const Natural & rhs); //!< *= rhs
            Natural &      operator *= (const natural_t rhs); //!< *= rhs
            Natural        sqr() const;                       //!< this * this


            //__________________________________________________________________
            //
            //
            // Division
            //
            //__________________________________________________________________

            //! Division function
            static Natural Div(const Natural &num, const Natural &den);
            friend Natural operator/(const Natural &, const Natural &);
            friend Natural operator/(const natural_t, const Natural &);
            friend Natural operator/(const Natural &, const natural_t);
            Natural & operator/=(const Natural &);
            Natural & operator/=(const natural_t);

            //__________________________________________________________________
            //
            //
            // Modulus
            //
            //__________________________________________________________________

            //! Modulus function
            static Natural Mod(const Natural &num, const Natural &den);
            friend Natural operator%(const Natural &, const Natural &);
            friend Natural operator%(const natural_t, const Natural &);
            friend Natural operator%(const Natural &, const natural_t);
            Natural & operator%=(const Natural &);
            Natural & operator%=(const natural_t);

            //__________________________________________________________________
            //
            //
            // Bits
            //
            //__________________________________________________________________
            Natural(const Exp2_ &, const size_t nbit);  //!< 2^nbit

            //__________________________________________________________________
            //
            // shr
            //__________________________________________________________________
            Natural & _shr(const size_t nbit) noexcept; //!< in place right shift
            Natural & _shr() noexcept;                  //!< in place right shift 1
            Natural   shr(const size_t nbit) const;     //!< this >> nbit

            friend Natural operator>>(const Natural &,const size_t); //!< this.shr(nbit)
            Natural &      operator>>=(const size_t) noexcept;       //!< this._shr(nbit)

            //__________________________________________________________________
            //
            // shl
            //__________________________________________________________________
            Natural        shl(const size_t nbit) const;             //!< left shift
            friend Natural operator<<(const Natural &,const size_t); //!< left shift
            Natural &      operator<<=(const size_t);                //!< left shift

            //__________________________________________________________________
            //
            //
            // Calculus
            //
            //__________________________________________________________________
            Natural sqrt() const;
            Natural abs()  const;

            //__________________________________________________________________
            //
            //
            // String conversion
            //
            //__________________________________________________________________
            String toHex() const;
            String toDec() const;

        private:
            Natural(Block * const, const AsBlock_ &);
            mutable BlockPtr block;
            mutable MutexPtr mutex;

            static Block *Add(Block &lhs, Block &   rhs);                      //!< default add
            static Block *Add(Block &lhs, natural_t rhs);                      //!< hybrid default add

            static Block *Sub(Block &   lhs, Block &   rhs, const Ops addOps); //!< lhs-rhs
            static Block *Sub(Block &   lhs, natural_t rhs);                   //!< lhs-rhs
            static Block *Sub(natural_t lhs, Block &   rhs);                   //!< lhs-rhs

            static Block *Mul(Block &lhs, Block &   rhs);                      //!< default add
            static Block *Mul(Block &lhs, natural_t rhs);                      //!< lhs*rhs

        };

    }

}

#endif
