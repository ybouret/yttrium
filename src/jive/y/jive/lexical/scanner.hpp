//! \file

#ifndef Y_Lexical_Scanner_Included
#define Y_Lexical_Scanner_Included 1

#include "y/jive/lexical/action.hpp"
#include "y/jive/pattern/dictionary.hpp"
#include "y/jive/lexical/unit.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Lexical
        {

         
            //__________________________________________________________________
            //
            //
            //
            //! Lexical Scanner: produce units from Source
            //
            //
            //__________________________________________________________________
            class Scanner : public Entity
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup with identifier
                template <typename LABEL>
                explicit Scanner(const LABEL &usr) :
                Entity(usr),
                dict(0),
                code( Initialize( *name, Coerce(dict) ) )
                {
                }

                //! cleanup
                virtual ~Scanner() noexcept;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________


                template <typename ID, typename RX, typename HOST, typename METHOD>
                inline void operator()(const ID &id, const RX &rx, HOST &host, METHOD meth)
                {
                    const Motif           motif( RegExp::Compile(rx, & *dict) );
                    const Callback        doing( &host, meth );
                    const Action::Pointer which( new Action(id,motif,doing)   );
                    noFragile(which->name,motif);
                    append(which);
                }

                unsigned produce(const Token &)
                {
                    return 0;
                }


                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                Dictionary * const dict; //!< internal dictionary

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scanner);
                class Code;
                Code *code;
                static Code *Initialize(const String &, Dictionary * &);
                void         append(const Action::Pointer &which);
                void         noFragile(const Tag &id, const Motif &motif) const;
            };
        }
        
    }
}

#endif
