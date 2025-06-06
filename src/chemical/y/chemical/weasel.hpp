//! \file


#ifndef Y_Chemical_Weasel_Included
#define Y_Chemical_Weasel_Included 1

#include "y/chemical/formula.hpp"
#include "y/chemical/reactive/equilibria.hpp"
#include "y/chemical/plexus/repertory.hpp"

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
            static const char * const      StringID;       //!< "String";
            class Parser;
            class FormulaToText;
            class FormulaToHTML;
            class Code;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            XNode *        parse(Lingo::Module * const);          //!< parse and preprocess

            //__________________________________________________________________
            //
            //
            // Formula/Species
            //
            //__________________________________________________________________
            Formula        parseFormula1(Lingo::Module * const);  //!< from single formula
            XNode *        singleFormula(Lingo::Module * const);  //!< from single formula
            const String * formulaToText(const Formula &, int &); //!< univocal name from parsed formula
            const String * formulaToHTML(const Formula &);        //!< univocal html from parsed formula
            xreal_t        eval(const String &);                  //!< use Lua to eval numerical expression

            //__________________________________________________________________
            //
            //
            // Equilibrium
            //
            //__________________________________________________________________
            Equilibrium  * compile(Library &     lib,
                                   XNode * const eNode,
                                   const size_t  eIndx); //!< compile equilibrium from parsed node

            

            //__________________________________________________________________
            //
            //
            // High-Level API
            //
            //__________________________________________________________________

            //! use root's content to update library and equilibria
            void     update(Library    &  lib,
                            Equilibria &  eqs,
                            Repertory  &  rep,
                            XNode * const root);


            //! update from parsed module
            void operator()(Library    &  lib,
                            Equilibria &  eqs,
                            Repertory  &  rep,
                            Lingo::Module * const);

            //! append Lua code to internal VM
            Weasel & operator<<(const String &luaCode);


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

