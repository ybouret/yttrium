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
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const      CallSign;       //!< "Weasel"
            static const AtExit::Longevity LifeTime = 100; //!< lifetime
            static const char * const      Coef;           //!< "Coef"
            static const char * const      RegExp;         //!< "RegExp";
            class Parser;
            class FormulaToText;
            class FormulaToSpec;
            class Code;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            XNode *        parse(Lingo::Module * const);          //!< parse and preprocess
            Formula        parseFormula1(Lingo::Module * const);  //!< from single formula
            const String * formulaToText(const Formula &, int &); //!< univocal name from parsed formula
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Weasel);
            
            explicit Weasel();
            virtual ~Weasel() noexcept;
            friend class Singleton<Weasel>;
            Code * const code;
            
        };

        

    }
}

#endif

