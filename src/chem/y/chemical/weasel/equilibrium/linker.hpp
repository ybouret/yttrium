

//! \file

#ifndef Y_Chemical_Equilibrium_Linker_Included
#define Y_Chemical_Equilibrium_Linker_Included 1

#include "y/chemical/reactive/equilibria.hpp"
#include "y/chemical/weasel/parser.hpp"

#include "y/lingo/syntax/translator.hpp"
#include "y/sequence/vector.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //! convert EQUILIBRIUM tree into equilibrium
        class Equilibrium:: Linker :  protected Lingo::Syntax::Translator
        {
        public:
            explicit Linker(const Weasel::Parser &);
            virtual ~Linker() noexcept;

            void process(XTree   &   tree,
                         Library &   lib,
                         Equilibria &eqs);

            String                         eqName;
            String                         spName;
            unsigned                       stoich;
            Actor::List                    actors;
            Actor::List                    reac;
            Actor::List                    prod;
            String                         Kstr;
            Library *                      theLib;
            Equilibria *                   theEqs;
            const Lingo::Syntax::Terminal  SPECIES;
            const Hashing::Perfect        &hashAct;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Linker);
            void replaceFormulae(XNode &parent, Library &lib);
            void preProcess(XTree &tree, Library &lib);

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
