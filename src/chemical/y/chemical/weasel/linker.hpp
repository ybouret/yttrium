
//! \file

#ifndef Y_Chemical_Weasel_Linker_Included
#define Y_Chemical_Weasel_Linker_Included 1

#include "y/jive/syntax/translator.hpp"
#include "y/chemical/library.hpp"
#include "y/sequence/vector.hpp"
#include "y/chemical/reactive/actors.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Weasel
        {

            class Linker : public Jive::Syntax::Translator
            {
            public:
                explicit Linker();
                virtual ~Linker() noexcept;

                void operator()(const Jive::Syntax::XNode &usrAST,
                                Library                   &usrLib);

                Vector<String>    UUID;
                Vector<int>       Z;
                Vector<Species *> SP;
                Vector<unsigned>  CF;
                Actor::List       actors;
                Actor::List       REAC;
                Actor::List       PROD;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Linker);
                Library *lib;
                virtual void initialize() noexcept;

                void onUUID(const Jive::Token &);
                void onDROP(const Jive::Token &);
                void onCF(const Jive::Token &);
                void onPOS(const size_t);
                void onNEG(const size_t);
                void onSP(const size_t);
                void onCHEMICAL(const size_t);
                void onACTOR(const size_t n);
            };

        }

    }

}

#endif


