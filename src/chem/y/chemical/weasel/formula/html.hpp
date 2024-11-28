
//! \file

#ifndef Y_Chemical_Formula_HTML_Included
#define Y_Chemical_Formula_HTML_Included 1

#include "y/chemical/formula.hpp"
#include "y/chemical/weasel/parser.hpp"
#include "y/lingo/syntax/translator.hpp"
#include "y/sequence/vector.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Formula:: ToHTML : public Lingo::Syntax::Translator
        {
        public:
            explicit ToHTML(const Weasel::Parser &parser);
            virtual ~ToHTML() noexcept;

            Strings elements;
            Strings integers;
            Strings zcharges;

            void clear() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ToHTML);

            virtual void init();
            virtual void quit();

            void onELEMENT(const Lexeme &);
            void onINTEGER(const Lexeme &);
            void onZCHARGE(const Lexeme &);

            void onFORMULA(const size_t);
            void onSTOCHIO(const size_t);
            void onZ(const size_t);
        };

    }

}

#endif

