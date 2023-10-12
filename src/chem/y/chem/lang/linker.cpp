#include "y/chem/lang/linker.hpp"
#include "y/type/temporary.hpp"
#include "y/system/exception.hpp"
#include "y/apex/natural.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Linker:: ~Linker() noexcept
        {
        }

        Linker:: Linker() noexcept :
        Jive::Syntax::Translator(),
        pLib(0),
        pEqs(0),
        UID(),
        SPZ(),
        SPR(),
        COF(),
        ACL(),
        ACR(),
        ACP(),
        KEQ(),
        RXP()
        {

            forTerminal("UID",   *this, & Linker:: onUID);
            forTerminal("+",     *this, & Linker:: onSGN);
            forTerminal("-",     *this, & Linker:: onSGN);
            forTerminal("COEFF", *this, & Linker:: onCOEFF);
            forTerminal("K",     *this, & Linker:: onK);
            forTerminal("RXP",   *this, & Linker:: onRXP);

            forInternal("Z+",      *this, & Linker:: onZP);
            forInternal("Z-",      *this, & Linker:: onZM);
            forInternal("SPECIES", *this, & Linker:: onSpecies);
            forInternal("ACTOR",   *this, & Linker:: onACTOR);
            forInternal("REAC",    *this, & Linker:: onREAC);
            forInternal("PROD",    *this, & Linker:: onPROD);
            forInternal("EQ",      *this, & Linker:: onEQ);
            forInternal("Weasel",  *this, & Linker:: onEND);

        }

        void Linker:: clean() noexcept
        {
            UID.free();
            SPZ.free();
            SPR.free();
            COF.free();
            ACL.free();
            ACR.free();
            ACP.free();
            KEQ.free();
        }

        void Linker:: initialize()
        {
            clean();
            RXP.free();
        }

        void Linker:: onEND(const size_t)
        {
            clean();
        }

        void Linker:: onUID(const Token &token)
        {
            UID << token.toString();
            indent() << "UID=" << UID << std::endl;
        }

        void Linker:: onZP(const size_t z)
        {
            SPZ << int(z);
            indent() << "SPZ=" << UID << std::endl;
        }

        void Linker:: onZM(const size_t z)
        {
            SPZ << -int(z);
            indent() << "SPZ=" << SPZ << std::endl;
        }

        void Linker:: onSGN(const Token &)
        {

        }

        void Linker:: onCOEFF(const Token &coeff)
        {
            apn res = 0;
            for(const Jive::Char *ch=coeff.head;ch;ch=ch->next)
            {
                res *= 10;
                res += (**ch) - '0';
            }
            COF << res.cast<unsigned>("COEFF");
            indent() << "COF=" << COF << std::endl;
        }

        void Linker:: onACTOR(const size_t n)
        {
            assert(SPR.size>0);
            const Species &sp = SPR.pullTail();
            unsigned       nu = 1;

            switch(n)
            {
                case 1: // no coeff
                    break;

                case 2: assert(COF.size>0);
                    nu = COF.pullTail();
                    break;

                default:
                    throw Specific::Exception("Linker::Actor","invalid arguments");
            }
            const Actor ac(sp,nu);
            ACL << ac;
            indent() << "ACL=" << ACL << std::endl;
        }



        void Linker:: onSpecies(const size_t n)
        {
            assert(UID.size>0);

            // initialize
            String name = UID.pullTail(); 
            int    z    = 0;

            // select charge
            switch(n)
            {
                case 1: // no charge
                    break;

                case 2: // with charge
                    assert(SPZ.size>0);
                    z = SPZ.pullTail();
                    break;

                default:
                    throw Specific::Exception("Linker::Species","invalid arguments");
            }

            // modify name
            switch( Sign::Of(z) )
            {
                case Negative:
                    for(int i=-z;i>0;--i) name += '-';
                    break;
                case __Zero__:
                    break;
                case Positive:
                    for(int i=z;i>0;--i) name += '+';
                    break;
            }

            assert(pLib);

            // query/create species
            SPR << (*pLib)(name,z);

            indent() << "SPR=" << SPR << std::endl;

        }


        void Linker:: extract(Actors &dest, const size_t n)
        {
            assert(ACL.size>=n);
            for(size_t i=n;i>0;--i)
            {
                const Actor ac = ACL.pullTail();
                dest.pushHead( new Actor(ac) );
            }
        }


        void Linker:: onPROD(const size_t n)
        {
            assert(ACL.size>=n);
            Actors prod;
            extract(prod,n);
            ACP << prod;
            indent() << "ACP=" << ACP << std::endl;
        }

        void Linker:: onREAC(const size_t n)
        {
            assert(ACL.size>=n);
            Actors reac;
            extract(reac,n);
            ACR << reac;
            indent() << "ACR=" << ACR << std::endl;
        }

        void Linker:: onK(const Token &token)
        {
            const String s = token.toString(1,1);
            KEQ << s;
            indent() << "KEQ=" << KEQ << std::endl;
        }

        void Linker:: onRXP(const Token &token)
        {
            assert(token.size>=2);
            const String rxp = token.toString(1,0);
            RXP << rxp;
            indent() << "RXP=" << RXP << std::endl;
        }

        void Linker:: onEQ(const size_t n)
        {
            if(4!=n)
                throw Specific::Exception("Linker::Equilibrium","invalid arguments");

            // select data
            const String &eid  = **UID.tail;
            const Actors &reac = **ACR.tail;
            const Actors &prod = **ACP.tail;
            const String &Kstr = **KEQ.tail;

            indent() << " (*) " << eid << " : " << reac << " <=> " << prod << " : '" << Kstr << "'" << std::endl;

            if(reac.size+prod.size <=0 ) throw Specific::Exception(eid.c_str(),"empty equilibrium");
            if(Kstr.size()<=0)           throw Specific::Exception(eid.c_str(),"empty constant string");

            AutoPtr<Equilibrium> pEq = 0;


            if(isdigit(Kstr[1]))
            {
                const double k = pEqs->vm->eval<lua_Number>(Kstr);
                pEq = new ConstEquilibrium(eid,k);
                goto ASSEMBLE;
            }

            throw Specific::Exception(eid.c_str(),"unhandle K='%s'", Kstr.c_str());
        ASSEMBLE:
            assert(pEq.isValid());
            {
                Equilibrium &eq = *pEq;
                for(const Actor *a=reac.head;a;a=a->next) eq(-int(a->nu),a->sp);
                for(const Actor *a=prod.head;a;a=a->next) eq(a->nu,a->sp);
            }

            assert(0!=pEqs);

            (*pEqs)(pEq.yield());
            


            // cleanup
            UID.cutTail();
            ACR.cutTail();
            ACP.cutTail();
            KEQ.cutTail();
        }

        void Linker:: operator()(const XNode   &root,
                                 Library        &lib,
                                 LuaEquilibria  &eqs)
        {
            const Temporary<Library*>       tmpLib(pLib,&lib);
            const Temporary<LuaEquilibria*> tmpEqs(pEqs,&eqs);
            //translate(root,Jive::Syntax::Permissive);
            translate(root,Jive::Syntax::Restricted);

        }

    }

}

