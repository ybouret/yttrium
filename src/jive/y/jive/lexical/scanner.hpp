//! \file

#ifndef Y_Jive_Lexical_Scanner_Included
#define Y_Jive_Lexical_Scanner_Included 1

#include "y/jive/lexical/action.hpp"
#include "y/jive/pattern/dictionary.hpp"
#include "y/jive/lexical/unit.hpp"

namespace Yttrium
{
    class XMLog;

    namespace Jive
    {

        namespace Lexical
        {
            //__________________________________________________________________
            //
            //! probe algorithm return value
            //__________________________________________________________________
            enum ReturnValue
            {
                ReturnSuccess, //!< valid action
                ReturnFailure, //!< no action, BUT still data in source
                AtEndOfStream  //!< no action, AND no data in source
            };

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
                static bool Verbose; //!< scanning verbosity for internal XMLog
                typedef ArkPtr<String,Scanner> Pointer;
                
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup with identifier
                template <typename LABEL> inline
                explicit Scanner(const LABEL &label) :
                Entity(label), dict(0), code( Initialize( *name, Coerce(dict) ) )
                {
                }

                //! setup with identifier and using shared data from parent
                template <typename LABEL> inline
                explicit Scanner(const LABEL &label, const Scanner &parent) :
                Entity(label), dict(0), code( Initialize( *name, Coerce(dict), parent ) )
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


                //______________________________________________________________
                //
                //! creating a new action
                /**
                 \param id   name of the action/resulting Unit
                 \param rx   regular expression
                 \param host host object
                 \param meth host method pointer
                 */
                //______________________________________________________________
                template <typename ID, typename RX, typename HOST, typename METHOD>
                inline void operator()(const ID &id, const RX &rx, HOST &host, METHOD meth)
                {
                    const Motif     motif( RegExp::Compile(rx, & *dict) );
                    const Callback  doing( &host, meth );
                    Action::Pointer which( new Action(id,motif,doing)   );
                    submitCode(which);
                }


                //______________________________________________________________
                //
                //! create a simple action to produce or discard an unit
                //______________________________________________________________
                template <typename ID, typename RX>
                inline void operator()(const ID &id, const RX &rx, const bool emit = true)
                {
                    Scanner &self = *this;
                    self(id,rx,self, (emit ? & Scanner::produce : & Scanner::discard) );
                }

                //______________________________________________________________
                //
                //! lexical end line
                //______________________________________________________________
                template <typename ID, typename RX>
                inline void endl(const ID &id, const RX &rx, const bool emit = false)
                {
                    Scanner &self = *this;
                    self(id,rx,self, (emit ? & Scanner::newLineAndEmit : & Scanner::newLineAndDrop) );
                }

                //______________________________________________________________
                //
                //! iterative cleanup of all motifs
                //______________________________________________________________
                void cleanup() noexcept;

                //______________________________________________________________
                //
                //! iterative checking, mostly to debug
                //______________________________________________________________
                bool isClean() const noexcept;

                //______________________________________________________________
                //
                //! find next action from Source
                //______________________________________________________________
                ReturnValue probe(Source &, Action * &);

                //______________________________________________________________
                //
                // predefined callbacks
                //______________________________________________________________
                Message produce(const Token &);        //!< return LX_EMIT
                Message discard(const Token &);        //!< return LX_DROP
                Message newLineAndEmit(const Token &); //!< return LX_EMIT | LX_ENDL
                Message newLineAndDrop(const Token &); //!< return LX_DROP | LX_ENDL

                //______________________________________________________________
                //
                //! get internal log
                //______________________________________________________________
                XMLog & getXMLog() noexcept;

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
                static Code *Initialize(const String &, Dictionary * &, const Scanner &parent);
                void         submitCode(Action::Pointer &which);
            };

            
        }
        
    }
}

#endif
