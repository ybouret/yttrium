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

        class Linker : public Jive::Syntax::Translator, public Identifiable
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Vector<String>     Strings;
            typedef Vector<int>        Charges;
            typedef Vector<unsigned>   Coefs;

            class Players : public Object, public Actors
            {
            public:
                explicit Players() noexcept : Object(), Actors(), next(0), prev(0) {}
                virtual ~Players() noexcept;

                Players *next;
                Players *prev;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Players);
            };


            explicit Linker(const char * const id);
            virtual ~Linker() noexcept;

            virtual const char * callSign() const noexcept;

            void     release() noexcept;
            void     operator()(Jive::Syntax::XNode &tree, Library &lib, LuaEquilibria &eqs);



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Linker);
            const char * const call;
            Strings            uuid;
            Charges            algZ;
            Species::SoloList  spec;
            Coefs              coef;
            Actors             cast;
            CxxListOf<Players> reac;
            CxxListOf<Players> prod;
            Strings            Kstr;
            Library           *hLib;
            LuaEquilibria     *hEqs;

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
            void onCHEMICAL(const size_t) noexcept;
        };

    }

}

#endif
