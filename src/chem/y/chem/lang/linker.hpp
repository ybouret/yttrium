
//! \file

#ifndef Y_Chemical_Lang_Linker_Included
#define Y_Chemical_Lang_Linker_Included 1

#include "y/jive/syntax/translator.hpp"
#include "y/chem/species/library.hpp"
#include "y/chem/reactive/lua/equilibria.hpp"
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
            typedef Small::SoloHeavyList<const String> StringList;
            typedef StringList::NodeType               StringNode;

            explicit Linker() noexcept;
            virtual ~Linker() noexcept;

            void operator()(const XNode    &root,
                            Library        &lib,
                            LuaEquilibria  &eqs);

            //! clean all but RXP
            void clean() noexcept;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Linker);

            virtual void initialize();    //!< reset all internal data
            
            void onUID(const Token &);    //!< store on UID
            void onSGN(const Token &);    //!< drop sign
            void onZP(const size_t);      //!< store Z+
            void onZM(const size_t);      //!< store Z-
            void onSpecies(const size_t); //!< query/create specues, store on SP
            void onCOEFF(const Token &);  //!< store info COEFF;
            void onACTOR(const size_t);   //!< store Actor
            void onREAC(const size_t);    //!< build some REACtants
            void onPROD(const size_t);    //!< build some PRODucts
            void onK(const Token &);      //!< store a new constant value
            void onEQ(const size_t n);    //!< create a new equilibrium
            void onRXP(const Token &);    //!< store RXP for further processing
            void onEND(const size_t);     //!< top level parser rule
        
            void extract(Actors &dest, const size_t n);


            Library                            *pLib;
            LuaEquilibria                      *pEqs;
            Small::SoloHeavyList<String>        UID; //!< identifier
            Small::SoloHeavyList<int>           SPZ; //!< speices charge
            Small::SoloLightList<const Species> SPR; //!< species repo
            Small::SoloHeavyList<unsigned>      COF; //!< stoichiometric coeffs
            Small::SoloHeavyList<Actor>         ACL; //!< actor list
            Small::SoloHeavyList<Actors>        ACR; //!< actors for reac
            Small::SoloHeavyList<Actors>        ACP; //!< actors for products
            StringList                          KEQ; //!< constant strings

        public:
            StringList                           RXP; //!< regular expressions to match built-in

        };

    }

}

#endif

