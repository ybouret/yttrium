

//! \file

#ifndef Y_Chemical_Equilibrium_Linker_Included
#define Y_Chemical_Equilibirum_Linker_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/weasel/parser.hpp"

#include "y/lingo/syntax/translator.hpp"
//#include "y/sequence/vector.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Equilibrium:: Linker :  public Lingo::Syntax::Translator
        {
        public:
            explicit Linker(const Weasel::Parser &);
            virtual ~Linker() noexcept;

            void preProcess(XTree &tree, Library &lib);

            const Lingo::Syntax::Terminal  SPECIES;
            const Hashing::Perfect        &actors;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Linker);
            void replaceFormulae(XNode &parent, Library &lib);
        };
    }

}

#endif
