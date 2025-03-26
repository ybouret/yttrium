
//! \file


#ifndef Y_Chemical_FormulaToText_Included
#define Y_Chemical_FormulaToText_Included 1

#include "y/chemical/weasel.hpp"
#include "y/chemical/weasel/formula/compiler.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Transform parsed formula into univocal test
        //
        //
        //______________________________________________________________________
        class Weasel:: FormulaToText : public FormulaCompiler
        {
        public:
            static const char * const CallSign; //!< "FormulaToText"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit FormulaToText();            //!< setup
            virtual ~FormulaToText() noexcept;   //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! build name and fetch
            const String *get(const XNode &node, int &z);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(FormulaToText);
            Strings         str;
            Strings         cof;
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
            void onFormula(const size_t) noexcept;
        };

    }

}

#endif
