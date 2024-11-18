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

            typedef Functor<void,TL1(const Lexeme &)> OnTerminal;
            typedef Functor<void,TL1(const size_t)>   OnInternal;


            class Translator
            {
            public:
                enum Policy
                {
                    Inflexible,
                    Permissive
                };

                typedef SuffixMap<const String,OnTerminal> OnTerminalMap;
                typedef SuffixMap<const String,OnInternal> OnInternalMap;


                explicit Translator();
                virtual ~Translator() noexcept;

                virtual void init();
                virtual void quit();

                void operator()(const XNode &root);

                std::ostream & indent() const;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Translator);
                void walk(const XNode * const);

                

                void onTerminal(const String &name,
                                const Lexeme &unit);

                void onInternal(const String &name,
                                const XList  &chld);

                OnTerminalMap tmap;
                OnInternalMap imap;

            public:
                const size_t  deep;
                bool          verbose;
            };
        }
    }
}

#endif

