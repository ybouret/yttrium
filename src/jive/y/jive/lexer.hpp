
//! \file

#ifndef Y_Jive_Lexer_Included
#define Y_Jive_Lexer_Included 1

#include "y/jive/lexical/scanner.hpp"

namespace Yttrium
{
    namespace Jive
    {

        typedef Lexical::Unit  Lexeme;    //!< alias
        typedef Lexical::Units Lexemes;   //!< alias

        class Lexer : public Lexical::Scanner
        {
        public:
         
            //! setup with identifier
            template <typename LABEL> inline
            explicit Lexer(const LABEL &usr) :
            Scanner(usr),
            app( Create(*this) )
            {
            }

            virtual ~Lexer() noexcept;

            Lexeme *get();
            void    put(Lexeme *) noexcept;
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Lexer);
            class App;
            App  *app;

            static App *Create(Scanner &self);
            

        };
    }

}

#endif

