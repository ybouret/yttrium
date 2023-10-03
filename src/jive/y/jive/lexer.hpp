
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
            // Definition
            //
            //__________________________________________________________________
            typedef void (Lexer::*Branch)(const String &); //!< branching proto

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            //! setup with identifier
            template <typename LABEL> inline
            explicit Lexer(const LABEL &usr) :
            Scanner(usr,Lexical::AcceptEndOfStream),
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
            void    set()         noexcept;  //!< full reset
            
            bool     owns(const Scanner &) const noexcept; //!< check lexer ownership
            Scanner *query(const String &s)      noexcept; //!< query by name

            //! create a zero-argument scanner
            template <typename SCANNER, typename ID>
            SCANNER & make(const ID &id)
            {
                SCANNER * ptr = new SCANNER(id);
                submit(ptr);
                return *ptr;
            }

            void jump_(const String &); //!< low-level jump to another scanner
            void call_(const String &); //!< low-level call to another scanner
            void back_();               //!< low-level back to calling


            template <
            typename PLUGIN,
            typename IDENTIFIER>
            void plug(const IDENTIFIER &id)
            {
                PLUGIN *plg = new PLUGIN(*this,id);
                submit(plg);
                
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Lexer);
            class App;
            App  *app;

            static App *Create(Scanner &self);
            void        submit(const Scanner::Pointer &ptr);
            void        submit(Scanner *); //!< submit a newly created scanner
        };
    }

}

#endif

