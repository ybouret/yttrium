
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
                static const uint32_t     UUID = Y_FOURCC('T','E','R','M'); //!< alias
                static const char * const CallSign;                         //!< Terminal

                //! Property for a terminal
                enum Property
                {
                    IsRegular, //!< used in AST
                    IsDivider  //!< helper only
                };


                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! create a new terminal
                template <typename ID>
                explicit Terminal(const ID      &tid,
                                  const Property ppty = IsRegular,
                                  const bool     uniq = false) :
                Rule(tid,UUID),
                property(ppty),
                univocal(uniq)
                {
                    I_am<Terminal>(this);
                }

                //! cleanup
                virtual ~Terminal() noexcept;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! accept a terminal: lexeme with same name
                virtual bool accepts(Y_JIVE_SYNTAX_RULE_API) const;
                

                virtual void endorse(AddressBook &book) const; //! insert this in book
                virtual void vizCore(OutputStream &)    const;
                virtual void vizLink(OutputStream &)    const;
                virtual bool isFrail()         const noexcept; //!< false

                const Property property; //!< Property
                const bool     univocal; //!< univocal from corresponding pattern            Y_XML_SECTION(code->xml, "CheckAccepted");


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Terminal);
            };

        }

    }

}

#endif

