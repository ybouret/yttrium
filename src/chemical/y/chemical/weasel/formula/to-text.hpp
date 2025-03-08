
//! \file


#ifndef Y_Chemical_FormulaToText_Included
#define Y_Chemical_FormulaToText_Included 1

#include "y/chemical/weasel.hpp"
#include "y/lingo/syntax/translator.hpp"
#include "y/sequence/vector.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Weasel:: FormulaToText : public Lingo::Syntax::Translator
        {
        public:
            explicit FormulaToText();
            virtual ~FormulaToText() noexcept;

            const String *get(const XNode &node, int &z);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(FormulaToText);
            Vector<String>  str;
            Vector<String>  cof;
            Vector<char>    sgn;
            AutoPtr<String> uid;
            int             zzz;

            void         resetVec() noexcept;
            void         resetAll() noexcept;
            virtual void init();
            virtual void quit();

            void onName(const Lexeme &);
            void onCoef(const Lexeme &);
            void onMult(const size_t );
            void onBody(const size_t );
            void onPos(const Lexeme &);
            void onNeg(const Lexeme &);
            void onZ(const size_t );
        };

    }

}

#endif
