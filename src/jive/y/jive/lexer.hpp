
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
            //typedef void (Lexer::*proc)(const String &);

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

            
            //! create a zero-argument plugin, derived from scanner
            template <typename PLUGIN, typename ID>
            PLUGIN & make(const ID &id)
            {
                PLUGIN *               plg = new PLUGIN(id);
                const Scanner::Pointer ptr(plg);
                submit(ptr);
                return *plg;
            }

            


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Lexer);
            class App;
            App  *app;

            static App *Create(Scanner &self);
            void        submit(const Scanner::Pointer &ptr);
        };
    }

}

#endif

