//! \file

#ifndef Y_Lingo_Syntax_Translator_Included
#define Y_Lingo_Syntax_Translator_Included 1

#include "y/lingo/syntax/xnode.hpp"
#include "y/associative/suffix/map.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            typedef Functor<void,TL1(const Lexeme &)> OnTerminal; //!< when a terminal is met
            typedef Functor<void,TL1(const size_t)>   OnInternal; //!< when an internal is met

            //__________________________________________________________________
            //
            //
            //! helper to setup/debug
            //
            //__________________________________________________________________
            enum TranslatorPolicy
            {
                Restricted, //!< will raise an error upon an unknown name
                Permissive  //!< will skip any unknown name
            };

            //__________________________________________________________________
            //
            //
            //
            //! fast, table-driven translator for stack-based design
            //
            //
            //__________________________________________________________________
            class Translator : public Entity
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Lingo::Lexeme                       Lexeme;         //!< alias
                typedef SuffixMap<const Caption,OnTerminal> OnTerminalMap_; //!< alias
                typedef SuffixMap<const Caption,OnInternal> OnInternalMap_; //!< alias
                Y_Derived(OnTerminalMap,Quantized,Standard);                //!< alias
                Y_Derived(OnInternalMap,Quantized,Standard);                //!< alias
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup
                template <typename NAME> inline
                explicit Translator(const NAME & _name) :
                Entity(_name,AsCaption),
                tmap( new OnTerminalMap() ),
                imap( new OnInternalMap() ),
                deep(0),
                policy(Restricted),
                verbose(false)
                {
                }
                
                virtual ~Translator() noexcept;   //!< cleanup

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
                virtual void init(); //!< initialize, default is do-nothing for standalone
                virtual void quit(); //!< quit, default is do-nothing for standalone

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                void operator()(const XNode &root); //!< walk down root
                std::ostream & indent() const;      //!< indent w.r.t depth

                //! register procedure for terminal
                void on(const Caption    & label,
                        const OnTerminal & tproc);

                //! register procedure for internal
                void on(const Caption    & label,
                        const OnInternal & iproc);

                //! helper to declare terminal procedure
                template <
                typename NAME,
                typename HOST,
                typename METH> inline
                void onTerminal(const NAME & _name,
                                HOST &       _host,
                                METH         _meth)
                {
                    const Caption    label(_name);
                    const OnTerminal tproc( &_host, _meth );
                    on(label,tproc);
                }

                //! helper to declare internal procedure
                template <
                typename NAME,
                typename HOST,
                typename METH> inline
                void onInternal(const NAME & _name,
                                HOST &       _host,
                                METH         _meth)
                {
                    const Caption    label(_name);
                    const OnInternal iproc( &_host, _meth );
                    on(label,iproc);
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Translator);
                void walk(const XNode * const);

                void onNotFound(const Caption &    label,
                                const char * const where) const;

                void pushTerminal(const Caption &tlabel,
                                  const Lexeme  &lexeme);

                void callInternal(const Caption &ilabel,
                                  const XList   &branch);

                AutoPtr<OnTerminalMap> tmap; //!< terminal map
                AutoPtr<OnInternalMap> imap; //!< internal map

            public:
                const size_t     deep;    //!< current depth
                TranslatorPolicy policy;  //!< policy, default is Restricted
                bool             verbose; //!< verbosity, default is false
            };

            //! helper to build translator
#define Y_Lingo_OnTerminal(TYPE,NAME) do { onTerminal(#NAME, *this, & TYPE:: on##NAME); } while(false)

            //! helper to build translator
#define Y_Lingo_OnInternal(TYPE,NAME) do { onInternal(#NAME, *this, & TYPE:: on##NAME); } while(false)


        }
    }
}

#endif

