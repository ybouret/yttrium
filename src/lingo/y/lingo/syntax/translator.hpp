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
                Inflexible, //!< will raise an error upon an unknown name
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
            class Translator
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef SuffixMap<const String,OnTerminal> OnTerminalMap; //!< alias
                typedef SuffixMap<const String,OnInternal> OnInternalMap; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Translator();            //!< setup empty, inflexible, not verbose
                virtual ~Translator() noexcept;   //!< cleanup

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
                virtual void init(); //!< initialize, default is do-nothing for standalone
                virtual void quit(); //!< quit, default is do-nothing for standaloen

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                void operator()(const XNode &root); //!< walk down root
                std::ostream & indent() const;      //!< indent w.r.t depth

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Translator);
                void walk(const XNode * const);

                void onNotFound(const String &     name,
                                const char * const text) const;

                void onTerminal(const String &name,
                                const Lexeme &unit);

                void onInternal(const String &name,
                                const XList  &chld);

                OnTerminalMap tmap; //!< terminal map
                OnInternalMap imap; //!< internal map

            public:
                const size_t     deep;    //!< current depth
                TranslatorPolicy policy;  //!< policy, default is Inflexible
                bool             verbose; //!< verbosity, default is false
            };
        }
    }
}

#endif

