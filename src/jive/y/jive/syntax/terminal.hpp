
//! \file

#ifndef Y_Jive_Syntax_Terminal_Included
#define Y_Jive_Syntax_Terminal_Included 1

#include "y/jive/syntax/rule.hpp"
#include "y/type/fourcc.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            //__________________________________________________________________
            //
            //
            //
            //! Terminal rule
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
                static const uint32_t UUID = Y_FOURCC('T','E','R','M'); //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! create a new terminal
                template <typename ID>
                explicit Terminal( const ID &id ) : Rule(id,UUID)
                {
                    I_am<Terminal>(this);
                }

                //! cleanup
                virtual ~Terminal() noexcept;

                //! accept a terminal: lexeme with same name
                virtual bool accepts(Y_JIVE_SYNTAX_RULE_API) const;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Terminal);
            };

        }

    }

}

#endif

