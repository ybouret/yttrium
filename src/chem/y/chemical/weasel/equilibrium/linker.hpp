

//! \file

#ifndef Y_Chemical_Equilibrium_Linker_Included
#define Y_Chemical_Equilibrium_Linker_Included 1

#include "y/chemical/weasel/formula/to-species.hpp"
#include "y/lingo/syntax/translator.hpp"

namespace Yttrium
{
    namespace Chemical
    {

      

        //______________________________________________________________________
        //
        //
        //
        //! convert EQUILIBRIUM tree into equilibrium
        //
        //
        //______________________________________________________________________
        class Equilibrium:: Linker :  protected Lingo::Syntax::Translator
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Linker(const Weasel::Parser &); //!< setup
            virtual ~Linker() noexcept;              //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            //! process EQUILIBRIUM to create a new equilibrium
            void process(XTree   &   tree,
                         Library &   lib,
                         Equilibria &eqs);

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
        private:
            String                         eqName;
            String                         spName;
            unsigned                       stoich;
            Actor::List                    actors;
            Actor::List                    reac;
            Actor::List                    prod;
            String                         Kstr;
            Library *                      theLib;
            Equilibria *                   theEqs;
            const FormulaToSpecies         preProcess;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Linker);

            virtual void init();
            virtual void quit();
            void         clear() noexcept;

            void onEQ(const Lexeme &);
            void onSPECIES(const Lexeme &);
            void onINTEGER(const Lexeme &);
            void onACTOR(const size_t);
            void onREAC(const size_t);
            void onPROD(const size_t);
            void onK(const Lexeme &);
            void onEQUILIBRIUM(const size_t);

            Equilibrium & newEquilibrium(); //!< based on Kstr

        };
    }

}

#endif
