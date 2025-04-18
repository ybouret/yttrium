
//! \file

#ifndef Y_Chemical_Weasel_Included
#define Y_Chemical_Weasel_Included 1

#include "y/chemical/library.hpp"
#include "y/chemical/reactive/equilibria.hpp"
#include "y/lingo/module.hpp"
#include "y/singleton.hpp"
#include "y/lua++/state.hpp"
#include "y/hashing/perfect.hpp"

namespace Yttrium
{
    namespace Chemical
    {

#define Y_Weasel_REAC 0 //!< hash value
#define Y_Weasel_PROD 1 //!< hash value


        //______________________________________________________________________
        //
        //
        //
        //! Weasel is Equilibria And Species Elementary Language
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
            class Parser;

            static const char * const      CallSign;                                 //!< "Weasel"
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 30; //!< alias
            static const char * const      SYMBOL;       //!< "<=>"
            static const char              COLON = ':';  //!< alias

            //! Arguments for Scheme
            struct  Args
            {
                const Readable<String> & arg; //!< parsed
                Library &                lib; //!< current library
                Equilibria &             eqs; //!< current equilibria
            };

            typedef Functor<void,TL1(Args&)>        Scheme; //!< alias
            typedef SuffixMap<const String,Scheme> Schemes; //!< alias



            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            XNode * parse(Lingo::Module * const);     //!< parse a full module

            XNode * parseFormula(const String &);     //!< parse a string to single FORMULA
            XNode * parseFormula(const char * const); //!< parse text to  single FORMULA
            void    setupFormula(Formula &formula);   //!< rebuild name + charge from FORMULA
            String  toHTML(const Formula &);          //!< convert formula to HTML label

            void    setupEquilibrium(XTree &tree, Library &lib, Equilibria &eqs); //!< link EQUILIBRIUM into eqs, filling lib with species
            XNode  *parseEquilibrium(const String &);                             //!< parse a string to EQUILIBRIUM
            void    queryEquilibrium(XTree &search, Library &, Equilibria &);     //!< search precompiled in EDB

            void    queryInstruction(XTree &instr, Library &, Equilibria &);      //!< process instructions


            //! parse species and equilibria from a module
            void operator()(Library &             lib,
                            Equilibria &          eqs,
                            Lingo::Module * const inp);

            //! record a scheme for a given instruction label
            template <typename LABEL, typename HOST, typename METH> inline
            void on(const LABEL &label, HOST &host, METH meth)
            {
                const Scheme scheme( &host, meth );
                record(label,scheme);
            }


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Lua::VM                luaVM;   //!< shared VM
            const Lingo::Caption   caption; //!< shared caption
            const Hashing::Perfect hashing; //!< "REAC" or "ACTOR"
            Schemes                schemes; //!< database of schemes for instructions

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Weasel);
            friend class Singleton<Weasel>;
            virtual ~Weasel() noexcept;
            explicit Weasel();

            XNode * parseSingle(const Lingo::Syntax::Rule &rule,
                                const String              &expr,
                                const char * const         func);

            void record(const String &label, const Scheme &scheme);
            void record(const char * const, const Scheme &);

            void executeLuaCode(Args &); //!< to make Lua Scheme

        };
    }

}

#endif

