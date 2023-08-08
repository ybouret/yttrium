
//! \file

#ifndef Y_Jive_Token_Included
#define Y_Jive_Token_Included 1

#include "y/jive/char.hpp"

namespace Yttrium
{
    namespace Jive
    {

        //______________________________________________________________________
        //
        //
        //
        //! List of Chars
        //
        //
        //______________________________________________________________________
        class Token : public Char::List
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Token() noexcept; //!< setup
            virtual ~Token() noexcept; //!< cleanup
            Token(const Token &);      //!< copy
            Y_OSTREAM_PROTO(Token);    //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! convert to string with optional clipping
            String toString(const size_t skip=0,const size_t trim=0) const;


        private:
            Y_DISABLE_ASSIGN(Token);
        };
    }

}

#endif

