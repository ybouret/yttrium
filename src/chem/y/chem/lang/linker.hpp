
//! \file

#ifndef Y_Chemical_Lang_Linker_Included
#define Y_Chemical_Lang_Linker_Included 1

#include "y/jive/syntax/translator.hpp"
#include "y/chem/species/library.hpp"
#include "y/chem/reactive/equilibria.hpp"
#include "y/data/small/heavy/list/solo.hpp"
#include "y/data/small/light/list/solo.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        
        class Linker : public Jive::Syntax::Translator
        {
        public:
            typedef Jive::Syntax::XNode XNode;
            typedef Jive::Syntax::XList XList;
            typedef Jive::Token         Token;

            explicit Linker() noexcept;
            virtual ~Linker() noexcept;

            void operator()(const XNode &root,
                            Library     &lib,
                            Equilibria  &eqs);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Linker);

            virtual void initialize(); //!< reset all internal data
            
            void onUID(const Token &); //!< store on UID
            void onSGN(const Token &); //!< drop sign
            void onZP(const size_t);      //!< store Z+
            void onZM(const size_t);      //!< store Z-
            void onSpecies(const size_t); //!< query/create specues, store on SP
            void onCOEFF(const Token &);  //!< store info COEFF;

            Library                      *pLib;
            Equilibria                   *pEqs;
            Small::SoloHeavyList<String>        UID;
            Small::SoloHeavyList<int>           Z;
            Small::SoloLightList<const Species> SP;
            Small::SoloHeavyList<unsigned>      COEFF;
            

        };

    }

}

#endif

