
//! \file

#ifndef Y_Apex_Natural_Included
#define Y_Apex_Natural_Included 1

#include "y/apex/number.hpp"
#include "y/apex/block/ptr.hpp"
#include "y/apex/block/mtx.hpp"
#include "y/ostream-proto.hpp"
#include "y/random/bits.hpp"
#include "y/type/signs.hpp"

namespace Yttrium
{
    namespace Apex
    {
        class Block;

        Y_SHALLOW_DECL(AsBlock); //!< alias

        //______________________________________________________________________
        //
        //
        //! list of AddOps
        //
        //______________________________________________________________________
#define Y_Apex_AddOps_List \
/**/           Add8_1,     \
/**/           Add8_2,     \
/**/           Add8_4,     \
/**/           Add4_1,     \
/**/           Add4_2,     \
/**/           Add2_1

        //______________________________________________________________________
        //
        //
        //! Named adding operation style
        //
        //______________________________________________________________________
        enum AddOps { Y_Apex_AddOps_List };


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
            static const unsigned     NumAddOps = (JigAPI::Plans * (JigAPI::Plans-1) ) >> 1; //!< card(AddOps)
            static const AddOps       AddOpsTable[NumAddOps]; //!< aliases
            static const char * const AddOpsLabel[NumAddOps]; //!< named
            static AddOps             AddOpsPrime;            //!< default Add/Sub style

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
            // Access
            //
            //__________________________________________________________________
            Lockable & operator*()  const noexcept; //!< access internal mutex
            Lockable * operator->() const noexcept; //!< access internal mutex
            uint64_t      lsw()     const noexcept; //!< least significant 64bits word
            Block &       _block()        noexcept; //!< internal block
            const Block & _block()  const noexcept; //!< internal const block

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
            static Block *Add(Block &lhs, Block &rhs, const AddOps addOps, uint64_t * const ell);

            friend Natural operator+(const Natural & lhs, const Natural & rhs); //!< lhs+rhs
            friend Natural operator+(const Natural & lhs, const natural_t rhs); //!< lhs+rhs
            friend Natural operator+(const natural_t lhs, const Natural & rhs); //!< lhs+rhs

            Natural & operator+=(const Natural & rhs); //!< += rhs
            Natural & operator+=(const natural_t rhs); //!< += rhs
            void      incr();                          //!< +=1
            Natural & operator++();                    //!< ++this
            Natural   operator++(int);                 //!< this++

            //__________________________________________________________________
            //
            //
            // Subtractions
            //
            //__________________________________________________________________
            friend Natural operator-(const Natural & lhs, const Natural & rhs); //!< lhs-rhs
            friend Natural operator-(const Natural & lhs, const natural_t rhs); //!< lhs-rhs
            friend Natural operator-(const natural_t lhs, const Natural & rhs); //!< lhs-rhs

            Natural & operator-=(const Natural & rhs); //!< -= rhs
            Natural & operator-=(const natural_t rhs); //!< -= rhs
            void      decr();                          //!< -=1
            Natural & operator--();                    //!< --this
            Natural   operator--(int);                 //!< this--

        private:
            Natural(Block * const, const AsBlock_ &);
            mutable BlockPtr block;
            mutable MutexPtr mutex;

            static Block *Add(Block &lhs, Block &   rhs); //!< default add
            static Block *Add(Block &lhs, natural_t rhs); //!< hybrid default add

            static Block *Sub(Block &   lhs, Block &   rhs, const AddOps addOps); //!< lhs-rhs
            static Block *Sub(Block &   lhs, natural_t rhs);                   //!< lhs-rhs
            static Block *Sub(natural_t lhs, Block &   rhs);                   //!< lhs-rhs
        };

    }

}

#endif
