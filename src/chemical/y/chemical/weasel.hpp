//! \file


#ifndef Y_Chemical_Weasel_Included
#define Y_Chemical_Weasel_Included 1

#include "y/chemical/formula.hpp"
#include "y/chemical/reactive/equilibrium.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Weasel is Equilibrium and Species Elementary Language
        //
        //
        //______________________________________________________________________
        class Weasel : public Singleton<Weasel>
        {
        public:
            static const char * const      CallSign;       //!< "Weasel"
            static const AtExit::Longevity LifeTime = 100; //!< lifetime
            static const char * const      Coef;           //!< "Coef"

            class Parser;
            class FormulaToText;
            class FormulaToSpec;
            class Code;

            XNode *        parse(Lingo::Module * const); //!< parse and preprocess
            Formula        parseFormula1(Lingo::Module * const);
            const String * formulaToText(const Formula &, int &);

        private:
            explicit Weasel();
            virtual ~Weasel() noexcept;
            friend class Singleton<Weasel>;
        };

    }
}

#endif

