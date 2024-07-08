
#include "y/chemical/weasel/linker.hpp"
#include "y/type/temporary.hpp"
#include "y/system/exception.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/container/algo/crop.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Weasel
        {
            
            Linker:: ~Linker() noexcept
            {
            }

            Linker:: Linker(const char * const _) :
            Jive::Syntax::Translator(),
            callSign(_),
            UUID(),
            Z(),
            SP(),
            CF(),
            actors(),
            REAC(),
            PROD(),
            K(),
            lib(0),
            eqs(0),
            rxp(0)
            {
                assert(0!=callSign);

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
                Y_Jive_OnTerminal(Linker,RX);

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
                                      LuaEquilibria             &usrEqs,
                                      Sequence<String>          *usrRxp)
            {
                const Temporary<Library*>           tempLib(lib,&usrLib);
                const Temporary<LuaEquilibria*>     tempEqs(eqs,&usrEqs);
                const Temporary<Sequence<String> *> tempRxp(rxp,usrRxp);

                translate(usrAST,Yttrium::Jive::Syntax::Permissive);

            }

            void Linker:: onUUID(const Jive::Token &t)
            {
                // push UUID
                UUID << t.toString();
            }

            void Linker:: onDROP(const Jive::Token &)
            {
                // drop sign
            }

            void Linker:: onPOS(const size_t n)
            {
                // store positive sign
                Z << int(n);
            }

            void Linker:: onNEG(const size_t n)
            {
                // store negative sign
                Z << -int(n);
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
                // terminate
            }

            void Linker:: onCF(const Jive::Token &t)
            {
                // convert digits to coefficient
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
                String kval = t.toString(1,1);
                Algo::Crop(kval,isspace);
                K << kval;
            }

            void Linker:: onEQ(const size_t)
            {
                assert(UUID.size()>0);
                const String name = UUID.pullTail();
                const String kval = K.pullTail();
                Equilibrium *eq   = 0;

                if(kval.size()<=0) throw Specific::Exception(callSign,"empty constant for <%s>", name.c_str());

                if(isdigit(kval[1]))
                {
                    // assume constant
                    eq = & eqs->newConstant(name,eqs->lvm->eval<real_t>(kval));
                }
                else
                {
                    // assume lua
                    eq = & eqs->newLua(name,kval);
                }

                //std::cerr << name << ":" << REAC << Equilibrium::Mark << PROD << ":" << kval << std::endl;

                for(const Actor *a=REAC.head;a;a=a->next)
                {
                    (*eq)(-int(a->nu),a->sp);
                }
                REAC.release();

                for(const Actor *a=PROD.head;a;a=a->next)
                {
                    (*eq)(int(a->nu),a->sp);
                }
                PROD.release();

                // update metrics
                eqs->update(*eq);

            }

            void Linker:: onRX(const Jive::Token &t)
            {
                const String rx = t.toString(1,0);
                if(!rxp) throw Specific::Exception(callSign, "illegal '@%s' presence at this stage", rx.c_str());
                (*rxp) << rx;
            }


        }
    }

}

