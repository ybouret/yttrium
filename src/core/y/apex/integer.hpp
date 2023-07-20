//! \file

#ifndef Y_Apex_Integer_Included
#define Y_Apex_Integer_Included 1

#include "y/apex/natural.hpp"

namespace Yttrium
{
    namespace Apex
    {
        //______________________________________________________________________
        //
        //
        //
        //! Drop-in Signed Type replacement
        //
        //
        //______________________________________________________________________
        class Integer : public Number
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Apex::Natural"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Integer();                             //!< zero
            virtual ~Integer() noexcept;           //!< cleanup
            Integer(const Integer &);              //!< copy
            Integer & operator=(const Integer &);  //!< assign
            Integer(const int64_t);                //!< setup integer
            Integer & operator=(const int64_t);    //!< assign integer
            Y_OSTREAM_PROTO(Integer);              //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void xch(Integer &)     noexcept;  //!< no-throw exchange
            void appendTo(IO::Chars &) const;  //!< append to chars


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const SignType s; //!< sign
            const Natural  n; //!< unsigned value
            
        };
    }

    typedef  Apex::Integer apz; //!< alias
}

#endif
