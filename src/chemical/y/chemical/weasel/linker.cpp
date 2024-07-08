
#include "y/chemical/weasel/linker.hpp"
#include "y/type/temporary.hpp"
#include "y/system/exception.hpp"
#include "y/text/ascii/convert.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Weasel
        {

            Linker:: ~Linker() noexcept
            {
            }

            Linker:: Linker() : 
            Jive::Syntax::Translator(),
            UUID(),
            Z(),
            SP(),
            CF(),
            actors(),
            REAC(),
            PROD(),
            K(),
            lib(0),
            eqs(0)
            {
                Y_Jive_OnTerminal(Linker,UUID);
                forTerminal("+", *this, & Linker::onDROP);
                forTerminal("-", *this, & Linker::onDROP);
                Y_Jive_OnTerminal(Linker,CF);
                Y_Jive_OnInternal(Linker,POS);
                Y_Jive_OnInternal(Linker,NEG);
                Y_Jive_OnInternal(Linker,SP);
                Y_Jive_OnInternal(Linker,CHEMICAL);
                Y_Jive_OnInternal(Linker,ACTOR);
                Y_Jive_OnInternal(Linker,REAC);
                Y_Jive_OnInternal(Linker,PROD);
                Y_Jive_OnTerminal(Linker,K);
                Y_Jive_OnInternal(Linker,EQ);

            }

            void Linker:: initialize() noexcept
            {
                UUID.free();
                Z.free();
                SP.free();
                CF.free();
                actors.release();
                REAC.release();
                PROD.release();
                K.free();
            }



            void Linker::  operator()(const Jive::Syntax::XNode &usrAST,
                                      Library                   &usrLib,
                                      LuaEquilibria             &usrEqs)
            {
                const Temporary<Library*>       tempLib(lib,&usrLib);
                const Temporary<LuaEquilibria*> tempEqs(eqs,&usrEqs);

                translate(usrAST,Yttrium::Jive::Syntax::Permissive);

            }

            void Linker:: onUUID(const Jive::Token &t)
            {
                UUID << t.toString();
            }

            void Linker:: onDROP(const Jive::Token &)
            {
            }

            void Linker:: onPOS(const size_t n)
            {
                Z << int(n);
                std::cerr << "Z=" << Z << std::endl;
            }

            void Linker:: onNEG(const size_t n)
            {
                Z << -int(n);
                std::cerr << "Z=" << Z << std::endl;
            }

            // build species from name and optional sign
            void Linker:: onSP(const size_t n)
            {
                assert(1==n||2==n);

                //--------------------------------------------------------------
                // get UUID, initialize z
                //--------------------------------------------------------------
                String name = UUID.pullTail();
                int    z    = 0;

                //--------------------------------------------------------------
                // modify Z ?
                //--------------------------------------------------------------
                if(n==2)
                {
                    z = Z.pullTail();
                }

                switch(Sign::Of(z))
                {
                    case __Zero__: break;
                    case Positive:
                        for(int i=0;i<z;++i) name += '+';
                        break;
                    case Negative:
                        for(int i=z;i<0;++i) name += '-';
                        break;
                }

                //--------------------------------------------------------------
                // on the fly create/collect species
                //--------------------------------------------------------------
                const Species &sp = (*lib)(name,z);
                SP << (Species *)&sp;
            }


            void Linker:: onCHEMICAL(const size_t)
            {
            }

            void Linker:: onCF(const Jive::Token &t)
            {
                const String cfs = t.toString();
                CF << ASCII::Convert::To<unsigned>(cfs,"CF");
            }

            void Linker:: onACTOR(const size_t n)
            {
                assert(1==n||2==n);

                // get species+nu
                const Species * const sp = SP.pullTail();
                unsigned              nu = 1;
                if(2==n)
                {
                    nu = CF.pullTail();
                }

                // push new actor
                actors.pushTail( new Actor(nu,*sp) );
            }

            void Linker:: onREAC(size_t n)
            {
                assert(n<=actors.size);
                while(n-- > 0 )
                    REAC.pushHead( actors.popTail() );
            }

            void Linker:: onPROD(size_t n)
            {
                assert(n<=actors.size);
                while(n-- > 0 )
                    PROD.pushHead( actors.popTail() );
            }

            void Linker:: onK(const Jive::Token &t)
            {
                K << t.toString(1,1);
            }

            void Linker:: onEQ(const size_t)
            {
                assert(UUID.size()>0);
                const String name = UUID.pullTail();
                const String kval = K.pullTail();

                std::cerr << name << ":" << REAC << Equilibrium::Mark << PROD << ":" << kval << std::endl;
            }

        }
    }

}

