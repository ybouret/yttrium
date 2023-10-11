
//! \file

#ifndef Y_Chemical_Lang_Linker_Included
#define Y_Chemical_Lang_Linker_Included 1

#include "y/jive/syntax/translator.hpp"
#include "y/chem/species/library.hpp"
#include "y/chem/reactive/equilibria.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        
        class Linker : public Jive::Syntax::Translator
        {
        public:
            typedef Jive::Syntax::XNode XNode;
            typedef Jive::Syntax::XList XList;

            explicit Linker() noexcept;
            virtual ~Linker() noexcept;

            void operator()(const XNode &root,
                            Library     &lib,
                            Equilibria  &eqs);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Linker);
        };

    }

}

#endif

