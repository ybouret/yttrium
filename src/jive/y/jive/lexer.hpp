
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

        //______________________________________________________________________
        //
        //
        //
        //! A Lexer is a primary scanner with some plugins/replica
        //
        //
        //______________________________________________________________________
        class Lexer : public Lexical::Scanner
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            //! setup with identifier
            template <typename LABEL> inline
            explicit Lexer(const LABEL &usr) :
            Scanner(usr),
            app( Create(*this) )
            {
            }

            
            //! cleanup
            virtual ~Lexer() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Lexeme *get(Source &);           //!< get next lexeme from source
            void    put(Lexeme *) noexcept;  //!< put back a read lexeme
            void    restart()     noexcept;  //!< restart

            void jump(const String &); //!< jump to named scanner
            void call(const String &); //!< call named scanner
            void back();               //!< back from current scanner

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Lexer);
            class App;
            App  *app;

            static App *Create(Scanner &self);

        };
    }

}

#endif

