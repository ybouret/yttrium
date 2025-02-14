//! \file

#ifndef Y_Apex_Natural_Included
#define Y_Apex_Natural_Included 1

#include "y/apex/type/castable.hpp"
#include "y/apex/block/ptr.hpp"
#include "y/apex/block/mtx.hpp"
#include "y/ostream-proto.hpp"
#include "y/random/bits.hpp"
#include "y/type/signs.hpp"
#include "y/string.hpp"
#include "y/type/proxy.hpp"
#include <cfloat>

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



        //! create the guard name
#define Y_Apex_Lock__(X,Y) X##Y

        //! instantiate the guard name
#define Y_Apex_Lock_(HOST,ID) volatile Yttrium::Apex::Natural::AutoLock Y_Apex_Lock__(__guard,ID)(HOST)

        //! use a local AutoLock to lock HOST
#define Y_Apex_Lock(HOST) Y_Apex_Lock_(HOST,__LINE__)

        template <typename T> struct RealDigits;

        //! digits for float
        template <> struct RealDigits<float>       { static const unsigned Count = FLT_DIG; /*!< FLT_DIG */  };

        //! digits for double
        template <> struct RealDigits<double>      { static const unsigned Count = DBL_DIG;  /*!< DBL_DIG */};

        //! digits for long double
        template <> struct RealDigits<long double> { static const unsigned Count = LDBL_DIG; /*!< LDBL_DIG */};

        //______________________________________________________________________
        //
        //
        //
        //! Natural
        //
        //
        //______________________________________________________________________
        class Natural : public Castable, public Proxy<Block>
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
            Natural();                                      //!< default constructor
            Natural(const natural_t);                       //!< setup as natural
            Natural(Random::Bits &, const size_t);          //!< setup with given random bits
            virtual ~Natural() noexcept;                    //!< cleanup
            Natural(const Natural &);                       //!< duplicate
            Natural(const String &);                        //!< parse
            Natural & operator=(const Natural &);           //!< assign
            Natural & operator=(const natural_t)  noexcept; //!< assign natural
            Natural & operator=(const String &);            //!< parse
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
            Lockable &    access()  const      noexcept; //!< internal mutex access
            uint64_t      lsw()     const      noexcept; //!< least significant 64bits word
            Natural &     xch(Natural &)       noexcept; //!< no-throw exchange
            Natural &     plan(const Plan p)   noexcept; //!< set internal Plan
            Natural &     plan(Random::Bits &) noexcept; //!< set internal random plan

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

            bool isOdd()  const noexcept; //!< test if odd
            bool isEven() const noexcept; //!< test if even

            //__________________________________________________________________
            //
            //
            // Additions
            //
            //__________________________________________________________________

            //! low-level addition
            static Block *Add(Block &lhs, Block &rhs, const Ops addOps);

            friend Natural operator +  (const Natural & lhs, const Natural & rhs); //!< lhs+rhs
            friend Natural operator +  (const Natural & lhs, const natural_t rhs); //!< lhs+rhs
            friend Natural operator +  (const natural_t lhs, const Natural & rhs); //!< lhs+rhs
            Natural        operator +  () const;               //!< unary +
            Natural &      operator += (const Natural & rhs);  //!< += rhs
            Natural &      operator += (const natural_t rhs);  //!< += rhs
            Natural &      operator ++ ();                     //!< ++this
            Natural        operator ++ (int);                  //!< this++
            void           incr();                             //!< +=1
            SignType       _sgn() const noexcept;              //!< w.r.t 0

            //__________________________________________________________________
            //
            //
            // Subtractions
            //
            //__________________________________________________________________

            //! low-level subtraction
            static Block *Sub(Block &   lhs, Block &   rhs, const Ops addOps);
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


            static Block *BFA_Mul(Block &lhs, Block &rhs, const Ops mulOps); //!< low-level long multiplication with choice
            static Block *BFA_Mul(Block &lhs, Block &rhs);                   //!< default low-level long multiplication
            static Block *BFA_Sqr(Block &lhs);                               //!< default low-level square

            static Block *FFT_Mul(Block &lhs, Block &rhs); //!< low-level FFT multiplication
            static Block *FFT_Sqr(Block &lhs);             //!< low-level FFT square


            friend Natural operator *  (const Natural & lhs, const Natural & rhs); //!< lhs*rhs
            friend Natural operator *  (const Natural & lhs, const natural_t rhs); //!< lhs*rhs
            friend Natural operator *  (const natural_t lhs, const Natural & rhs); //!< lhs*rhs
            Natural &      operator *= (const Natural & rhs); //!< *= rhs
            Natural &      operator *= (const natural_t rhs); //!< *= rhs
            Natural        sqr() const;                       //!< this * this


            //__________________________________________________________________
            //
            //
            // Division/Modulus
            //
            //__________________________________________________________________

            //! helper to drive DivMod function
            struct DivModArg {
                Natural *rem; //!< optional remainder to store
            };

            //! swiss-army knife DivMod
            static Natural DivMod(const Natural &num, const Natural &den, DivModArg * const arg);


            //__________________________________________________________________
            //
            //
            // Division
            //
            //__________________________________________________________________

            //! Division function
            static Natural Div(const Natural &num, const Natural &den); //!< num/den
            friend Natural operator/(const Natural &, const Natural &); //!< division
            friend Natural operator/(const natural_t, const Natural &); //!< division
            friend Natural operator/(const Natural &, const natural_t); //!< division
            Natural & operator/=(const Natural &);                      //!< in place division
            Natural & operator/=(const natural_t);                      //!< in place division

            //__________________________________________________________________
            //
            //
            // Modulus
            //
            //__________________________________________________________________

            //! Modulus function
            static Natural Mod(const Natural &num, const Natural &den); //!< num%den
            friend Natural operator%(const Natural &, const Natural &); //!< modulus
            friend Natural operator%(const natural_t, const Natural &); //!< modulus
            friend Natural operator%(const Natural &, const natural_t); //!< modulus
            Natural & operator%=(const Natural &);                      //!< in place modulus
            Natural & operator%=(const natural_t);                      //!< in place modulus


            //__________________________________________________________________
            //
            //
            // Div/Mod
            //
            //__________________________________________________________________

            //! num = quot * den + rem
            static void Div(Natural &quot, Natural &rem, const Natural &num, const Natural &den);

            //__________________________________________________________________
            //
            //
            // GCD
            //
            //__________________________________________________________________
            static Natural GCD_(const Natural &a, const Natural &b); //!< GCD_(a>0,b>0)
            static Natural GCD(const Natural &a, const Natural &b);  //!< safe GCD
            static void    Simplify(Natural &num, Natural &den);     //!< simplify num/den
            static Natural LCM(const Natural &a, const Natural b);   //!< |ab|/GCD(a,b)

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
            Natural & _shr() noexcept;                  //!< in place right shift 1, aka /= 2
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
            Natural sqrt() const; //!< interger square-root
            Natural abs()  const; //!< |*this|
            static Natural Factorial(const natural_t);

            //__________________________________________________________________
            //
            //
            // String conversion
            //
            //__________________________________________________________________
            String    toHex() const; //!< to hexadecimal string
            String    toDec() const; //!< to decimal string
            Natural & toHex(const char * const text, const size_t size); //!< to Hex
            Natural & toDec(const char * const text, const size_t size); //!< to Dec


            //__________________________________________________________________
            //
            //
            // integral type conversion
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //
            //! floating type conversion
            //
            //__________________________________________________________________
            template <typename T>
            static inline T Ratio(const Natural &num, const Natural &den)
            {
                static const unsigned Digits = RealDigits<T>::Count;
                static const T        Factor = 256;
                static const T        Tenth  = 0.1;

                Natural N = num;
                Natural q = 0, r=0;

                T res = 0;

                Div(q,r,N,den);
                {
                    const Jig1 &jig = q.block->make<Plan1>();
                    const uint8_t * const b = jig.word;
                    size_t                n = jig.words;
                    while(n-- > 0 )
                    {
                        res *= Factor;
                        res += b[n];
                    }
                }


                T pos = Tenth;
                for(unsigned i=0;i<Digits;++i, pos *= Tenth)
                {
                    if(r->bits<=0) break;
                    N.xch(r);
                    N *= 10;
                    Div(q,r,N,den);
                    res += pos * static_cast<T>(q.block->make<Plan1>().word[0]);
                }

                return res;
            }


            //! try cast to given value
            template <typename T>
            bool tryCast(T &target) const
            {
                static const unsigned RawSize = sizeof(T);
                static const unsigned RawBits = RawSize << 3;
                static const bool     Signed  = IsSigned<T>::Value;
                static const unsigned MaxBits = Signed ? RawBits-1 : RawBits;
                
                if(block->bits>MaxBits) {
                    return false;
                }
                else
                {
                    Y_Apex_Lock(*this);
                    target = block->cast<T>();
                    return true;
                }
            }

            //! cast to integeral type
            template <typename T> inline
            T cast(const char * const variable) const
            {
                T res(0);
                if(!tryCast(res)) raiseCastOverflowFor(variable);
                return res;
            }

            static Natural FFT(const Natural &lhs, const Natural &rhs); //!< FFT product
            Natural(Block * const, const AsBlock_ &);                   //!< manual setting

        private:
            Y_PROXY_DECL();
            mutable BlockPtr block;
            mutable MutexPtr mutex;
            void   setBlockTo(const natural_t) noexcept; //!< set block content 

            static Block *Add(Block &lhs, Block &   rhs);                      //!< default add
            static Block *Add(Block &lhs, natural_t rhs);                      //!< hybrid default add

            static Block *Sub(Block &   lhs, natural_t rhs);                   //!< lhs-rhs
            static Block *Sub(natural_t lhs, Block &   rhs);                   //!< lhs-rhs

            static Block *Mul1(Block &lhs, natural_t rhs);                      //!< lhs*rhs

            
        };



    }

}

#endif
