//! \file

#ifndef Y_Lingo_Source_Included
#define Y_Lingo_Source_Included 1

#include "y/lingo/module.hpp"
#include "y/lingo/token.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        //______________________________________________________________________
        //
        //
        //
        //! Buffered Source of Chars
        //
        //
        //______________________________________________________________________
        class Source : public InputStream
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Lingo::Source"

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
            // Interface
            //
            //__________________________________________________________________
            virtual const char * callSign() const noexcept; //!< [Identifiable] CallSign
            virtual bool         query(char &);             //!< [Identifiable] extract from get()
            virtual void         store(const char);         //!< not allowed!

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Char *          get();                        //!< query a new char
            void            put(Char  *)       noexcept;  //!< unread a char
            void            put(Token &)       noexcept;  //!< unread a token
            void            dup(const Token &);           //!< unread a copy
            bool            ready();                      //!< check if char is available
            bool            guess(Token &token);          //!< guess next blank-separated token
            void            skip()         noexcept;      //!< skip next char if ready!
            void            skip(size_t n) noexcept;      //!< skip n<=cached()
            const Char *    peek()   const noexcept;      //!< next char if ready()
            size_t          cached() const noexcept;      //!< cache size
            void            newLine()      noexcept;      //!< propagate newLine to module
            bool            getch(Token &);               //!< try to read any next char
            const Context * operator->() const noexcept;  //!< current context

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Source);
            class Code;
            Code *code;
        };
    }

}


#endif

