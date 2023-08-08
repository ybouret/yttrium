
//! \file

#ifndef Y_Jive_Source_Included
#define Y_Jive_Source_Included 1

#include "y/jive/module.hpp"
#include "y/jive/token.hpp"

namespace Yttrium
{
    namespace Jive
    {

        class Source
        {
        public:
            explicit Source(Module *);
            virtual ~Source() noexcept;

            Char *get();                        //!< query a new char
            void  put(Char  *)       noexcept;  //!< unread a char
            void  put(Token &)       noexcept;  //!< unread a token
            void  dup(const Token &);           //!< unread a copy

            void  newLine() noexcept; //!< propagate newLine
            const Module & operator*() const noexcept;

            bool  guess(Token &token);

            const Module * operator->() const noexcept;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Source);
            class Code;
            Code *code;
        };
    }

}


#endif

