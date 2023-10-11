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
        pEqs(0)
        {

            forTerminal("UID",   *this, & Linker:: onUID);
            forTerminal("+",     *this, & Linker:: onSGN);
            forTerminal("-",     *this, & Linker:: onSGN);
            forTerminal("COEFF", *this, & Linker:: onCOEFF);

            forInternal("Z+", *this, & Linker:: onZP);
            forInternal("Z-", *this, & Linker:: onZM);
            forInternal("SPECIES", *this, & Linker::onSpecies);

        }

        void Linker:: initialize()
        {
            UID.free();
            Z.free();
            SP.free();
            COEFF.free();
        }

        void Linker:: onUID(const Token &token)
        {
            UID << token.toString();
            indent() << "UID=" << UID << std::endl;
        }

        void Linker:: onZP(const size_t z)
        {
            Z << int(z);
            indent() << "Z=" << UID << std::endl;
        }

        void Linker:: onZM(const size_t z)
        {
            Z << -int(z);
            indent() << "Z=" << Z << std::endl;
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
            COEFF << res.cast<unsigned>("COEFF");
            indent() << "COEFF=" << COEFF << std::endl;
        }

        void Linker:: onSpecies(const size_t n)
        {
            assert(UID.size>0);
            String name = UID.pullTail(); 
            int    z    = 0;
            switch(n)
            {
                case 1: // no charge
                    break;

                case 2: // with charge
                    assert(Z.size>0);
                    z = Z.pullTail();
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
            const Species &sp = (*pLib)(name,z);
            SP << sp;

            indent() << "SP=" << SP << std::endl;

        }


        void Linker:: operator()(const XNode &root,
                                 Library     &lib,
                                 Equilibria  &eqs)
        {
            const Temporary<Library*>    tmpLib(pLib,&lib);
            const Temporary<Equilibria*> tmpEqs(pEqs,&eqs);
            translate(root,Jive::Syntax::Permissive);
        }

    }

}

