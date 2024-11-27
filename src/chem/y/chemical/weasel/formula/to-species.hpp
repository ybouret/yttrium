//! \file
#ifndef Y_Chemical_Formula_To_Species_Included
#define Y_Chemical_Formula_To_Species_Included 1

#include "y/chemical/weasel/parser.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! AST Formula to Species transformation to EQUILIBRIUM tree
        //
        //
        //______________________________________________________________________
        class FormulaToSpecies
        {
        public:
            typedef Lingo::Lexeme Lexeme;                         //!< alias
            explicit FormulaToSpecies(const Weasel::Parser &);    //!< setup
            virtual ~FormulaToSpecies() noexcept;                 //!< cleanup
            void     operator()(XTree &tree, Library &lib) const; //!< apply

        private:
            Y_DISABLE_COPY_AND_ASSIGN(FormulaToSpecies);
            void subst(XNode &parent, Library &lib) const;

            const Lingo::Syntax::Terminal  SPECIES;
            const Hashing::Perfect &       hashing;

        };


    }

}


#endif
