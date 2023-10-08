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
        class Lexer;

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
            //! End Of Stream Policy
            //__________________________________________________________________
            enum EndOfStreamPolicy
            {
                RejectEndOfStream,
                AcceptEndOfStream
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
                static bool                    Verbose;    //!< scanning verbosity for internal XMLog
                typedef ArkPtr<String,Scanner> Pointer;    //!< alias
                static const char * const      BackPrefix; //!< to name back controls
                static const char * const      JumpPrefix; //!< to name jump controls
                static const char * const      CallPrefix; //!< to name call controls

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup with identifier
                template <typename LABEL> inline
                explicit Scanner(const LABEL             &label,
                                 const EndOfStreamPolicy  scheme = AcceptEndOfStream) :
                Entity(label),
                dict(0),
                root(0),
                _eos(scheme),
                code( Initialize( *name, Coerce(dict) ) )
                {
                }

                //! setup with identifier and using SHARED data from parent
                template <typename LABEL> inline
                explicit Scanner(const LABEL            &label,
                                 Scanner                &parent,
                                 const EndOfStreamPolicy scheme) :
                Entity(label),
                dict(0),
                root(&parent),
                _eos(scheme),
                code( Initialize( *name, Coerce(dict), parent ) )
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

                //! submit an action
                const Action &submitCode(Action::Pointer &which);



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
                inline const Action& operator()(const ID &id, const RX &rx, HOST &host, METHOD meth)
                {
                    const Motif     motif( RegExp::Compile(rx, & *dict) );
                    const Callback  doing( &host, meth );
                    Action::Pointer which( new Action(id,motif,doing)   );
                    return submitCode(which);
                }


                //______________________________________________________________
                //
                //! create a simple action to produce an unit
                //______________________________________________________________
                template <typename ID, typename RX>
                inline const Action &emit(const ID &id, const RX &rx)
                {
                    Scanner &self = *this;
                    return self(id,rx,self,& Scanner::produce);
                }

                //______________________________________________________________
                //
                //! create a simple action to discard an unit
                //______________________________________________________________
                template <typename ID, typename RX>
                inline const Action &drop(const ID &id, const RX &rx)
                {
                    Scanner &self = *this;
                    return self(id,rx,self,& Scanner::discard);
                }

                //______________________________________________________________
                //
                //! create a simple action to discard an unit, name = expression
                //______________________________________________________________
                template <typename RX>
                inline const Action &drop(const RX &rx)
                {
                    Scanner &self = *this;
                    return self(rx,rx,self,& Scanner::discard);
                }


                //______________________________________________________________
                //
                //! lexical end line
                //______________________________________________________________
                template <typename ID, typename RX>
                inline const Action &endl(const ID &id, const RX &rx, const bool emit = false)
                {
                    Scanner &self = *this;
                    return self(id,rx,self, (emit ? & Scanner::newLineAndEmit : & Scanner::newLineAndDrop) );
                }

                //______________________________________________________________
                //
                //! lexical end line, name=expression
                //______________________________________________________________
                template < typename RX>
                inline const Action &endl(const RX &rx, const bool emit = false)
                {
                    Scanner &self = *this;
                    return self(rx,rx,self, (emit ? & Scanner::newLineAndEmit : & Scanner::newLineAndDrop) );
                }


                //______________________________________________________________
                //
                //! create a coming back from this scanner within host's lexer
                //______________________________________________________________
                template <typename RX, typename HOST, typename METHOD>
                inline void back(const RX       &rx,
                                 Lexer          &lexer,
                                 HOST           &host,
                                 METHOD          meth)
                {
                    const Motif    motif( RegExp::Compile(rx, & *dict) );
                    const Callback leave(&host,meth);
                    backOn(motif,leave,lexer);
                }

                //______________________________________________________________
                //
                //! create a jump to a lexer[id] upon rx
                //______________________________________________________________
                template <typename ID, typename RX, typename HOST, typename METHOD>
                inline void jump(const ID &id,
                                 const RX &rx,
                                 Lexer    &lexer,
                                 HOST     &host,
                                 METHOD    meth)
                {
                    branch(id,rx,lexer,host,meth,false);
                }

                //______________________________________________________________
                //
                //! create a call to a lexer[id] upon rx
                //______________________________________________________________
                template <typename ID, typename RX, typename HOST, typename METHOD>
                inline void call(const ID &id,
                                 const RX &rx,
                                 Lexer    &lexer,
                                 HOST     &host,
                                 METHOD    meth)
                {
                    branch(id,rx,lexer,host,meth,true);
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
                Dictionary * const      dict;   //!< internal dictionary
                Scanner    * const      root;   //!< if root
                const EndOfStreamPolicy _eos;   //!< behavior on EOS

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scanner);
                class Code;
                Code *code;

                static Code *Initialize(const String &, Dictionary * &);
                static Code *Initialize(const String &, Dictionary * &, const Scanner &parent);

                void backOn(const Motif    &motif,
                            const Callback &leave,
                            Lexer          &lexer);

                void jumpOn(const Motif    &motif,
                            const Tag      &where,
                            const Callback &enter,
                            Lexer          &lexer);

                void callOn(const Motif    &motif,
                            const Tag      &where,
                            const Callback &enter,
                            Lexer          &lexer);



                template <typename ID, typename RX, typename HOST, typename METHOD>
                inline void branch(const ID &id,
                                   const RX &rx,
                                   Lexer    &lexer,
                                   HOST     &host,
                                   METHOD    meth,
                                   const bool isCall)
                {
                    const Motif    motif( RegExp::Compile(rx, & *dict) );
                    const Tag      where(id);
                    const Callback enter(&host,meth);
                    if(isCall) callOn(motif,where,enter,lexer);
                    else       jumpOn(motif,where,enter,lexer);
                }


            };

            
        }
        
    }
}

#endif
