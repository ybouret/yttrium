
//! \file

#ifndef Y_Jive_Token_Included
#define Y_Jive_Token_Included 1

#include "y/jive/char.hpp"

namespace Yttrium
{
    namespace Jive
    {


        class Token : public Char::List
        {
        public:
            explicit Token() noexcept;
            virtual ~Token() noexcept;
            Token(const Token &);
            Y_OSTREAM_PROTO(Token);

            String toString(const size_t skip=0,const size_t trim=0) const;


        private:
            Y_DISABLE_ASSIGN(Token);
        };
    }

}

#endif

