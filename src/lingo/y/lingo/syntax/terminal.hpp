
//! \file

#ifndef Y_Lingo_Syntax_Terminal_Included
#define Y_Lingo_Syntax_Terminal_Included 1

#include "y/lingo/syntax/rule.hpp"
#include "y/type/fourcc.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            //__________________________________________________________________
            //
            //
            //
            //! Terminal rule matching lexemes
            //
            //
            //__________________________________________________________________
            class Terminal : public Rule
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________

                //! Standard or Univocal termainl
                enum Kind
                {
                    Standard, //!< may take multiple form
                    Univocal  //!< may take only one form, expandable content
                };

                //! Semantic of Dividing
                enum Role
                {
                    Semantic, //!< keep in AST
                    Dividing  //!< remove from AST
                };

                //! identifier
                static const uint32_t UUID = Y_FOURCC('T','E','R','M');

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup and qualify
                template <typename NAME> inline
                explicit Terminal(const NAME &    _name,
                                  const Kind      _kind,
                                  const Role      _role,
                                  Grammar * const _host) :
                Rule(_name,UUID,_host),
                kind(_kind),
                role(_role)
                {
                    Y_Lingo_Syntax_Rule(Terminal);
                }

                //! cleanup
                virtual ~Terminal() noexcept;

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
                virtual bool         accepts(Y_Lingo_Syntax_Args) const; //!< accept matching name lexeme
                virtual bool         robust()                     const; //!< true
                virtual void         vizMark(OutputStream &)      const; //!< box, style...

                //______________________________________________________________
                //
                //
                // Method
                //
                //______________________________________________________________
                Lexeme * synthetic(const String &input);

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Kind kind; //!< manage content
                const Role role; //!< manage AST

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Terminal);
            };

        }

    }

}


#endif

