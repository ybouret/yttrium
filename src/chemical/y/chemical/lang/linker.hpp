//! \file

#ifndef Y_Chemical_Lang_Linker_Included
#define Y_Chemical_Lang_Linker_Included 1


#include "y/jive/syntax/translator.hpp"
#include "y/sequence/vector.hpp"
#include "y/chemical/reactive/equilibria/lua.hpp"
#include "y/chemical/species/library.hpp"



namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Linker converts AST into lib/eqs
        //
        //
        //______________________________________________________________________
        class Linker : public Jive::Syntax::Translator, public Identifiable
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Vector<String>     Strings; //!< alias
            typedef Vector<int>        Charges; //!< alias
            typedef Vector<unsigned>   Coefs;   //!< alias

            //! linked actors to store reac/prod
            class Players : public Object, public Actors
            {
            public:
                explicit Players() noexcept; //!< setup
                virtual ~Players() noexcept; //!< cleanup

                Players *next; //!< for list
                Players *prev; //!< for list

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Players);
            };


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Linker(const char * const id); //!< setup with persistent id
            virtual ~Linker() noexcept;             //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual const char * callSign() const noexcept; //!< identifier
            void                 release()        noexcept; //!< clean all

            //! operates
            void     operator()(Jive::Syntax::XNode &tree,
                                Library             &lib,
                                LuaEquilibria       &eqs,
                                Sequence<String> * const rxp);



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Linker);
            const char * const call;
            Strings            uuid;
            Charges            algZ;
            SList              spec;
            Coefs              coef;
            Actors             cast;
            CxxListOf<Players> reac;
            CxxListOf<Players> prod;
            Strings            Kstr;
            Library           *hLib;
            LuaEquilibria     *hEqs;
            Sequence<String>  *hRxp;
            
            void onUUID( const Jive::Token &);
            void onPLUS( const Jive::Token &);
            void onMINUS(const Jive::Token &);
            void onZPOS(size_t);
            void onZNEG(size_t);
            void onSPECIES(const size_t);
            void onCOEF(const Jive::Token &t);
            void onACTOR(const size_t);
            void gather(Players * const,size_t) noexcept;
            void onREAC(const size_t);
            void onPROD(const size_t);
            void onK(const Jive::Token &);
            void onEQ(const size_t);
            void onRXP(const Jive::Token &);
            void onCHEMICAL(const size_t) noexcept;
        };

    }

}

#endif
