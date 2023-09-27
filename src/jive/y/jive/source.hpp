//! \file

#ifndef Y_Jive_Source_Included
#define Y_Jive_Source_Included 1

#include "y/jive/module.hpp"
#include "y/jive/token.hpp"

namespace Yttrium
{
    namespace Jive
    {

        //______________________________________________________________________
        //
        //
        //
        //! Buffered Source of Chars
        //
        //
        //______________________________________________________________________
        class Source
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Source(Module *);  //!< setup from module
            virtual ~Source() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Char *get();                        //!< query a new char
            void  put(Char  *)       noexcept;  //!< unread a char
            void  put(Token &)       noexcept;  //!< unread a token
            void  dup(const Token &);           //!< unread a copy

            bool    ready();                 //!< check if char is available
            bool    guess(Token &token);     //!< guess next blank-separated token
            void    skip()         noexcept; //!< skip next char if ready!
            void    skip(size_t n) noexcept; //!< skip n<=cached()
            uint8_t peek()   const noexcept; //!< next content if ready()
            size_t  cached() const noexcept; //!< cache size
            void    newLine()      noexcept; //!< propagate newLine to module

            //! read-only inner module access
            const Module * operator->() const noexcept;
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Source);
            class Code;
            Code *code;
        };
    }

}


#endif

