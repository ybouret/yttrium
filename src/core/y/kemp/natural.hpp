//! \file

#ifndef Y_Kemp_Natural_Included
#define Y_Kemp_Natural_Included 1

#include "y/kemp/number.hpp"
#include "y/kemp/types.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        class Element;

        //______________________________________________________________________
        //
        //
        //
        //! Natural, unsing internal Element
        //
        //
        //______________________________________________________________________
        class Natural : public Number
        {
        public:    
            //__________________________________________________________________
            //
            //
            // definitions
            //
            //__________________________________________________________________
            static Ops Strategy; //!< global strategy

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Y_OSTREAM_PROTO(Natural);    //!< display decimal/hexadecimal
            virtual ~Natural() noexcept; //!< cleanup

            Natural();                //!< default setup = 0
            Natural(const Natural &); //!< copy
            Natural(const uint64_t);  //!< set to qword

            Natural(const size_t nbits, Random::Bits &);   //!< exactly nbits random bits
            Natural(const TwoToThe_ &, const size_t ibit); //!< 2^ibit

            Natural &operator=(const Natural &); //!< assign
            Natural &operator=(const uint64_t);  //!< assign qword

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            void xch(Natural &) noexcept; //!< noexcept exchange content
            String toHex()         const; //!< render to hexadecimal


        private:
            mutable Element *code;
        };

    }
    typedef Kemp::Natural apn; //!< alias
    
}

#endif

