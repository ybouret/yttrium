
#include "y/chemical/plexus/equalizer/faders.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * Faders:: TextFlag(const unsigned flag) noexcept
        {
            switch(flag)
            {
                case BALANCED: return "BALANCED";
                case BAD_REAC: return "BAD REAC";
                case BAD_PROD: return "BAD PROD";
                case BAD_BOTH: return "BAD BOTH";
                default:
                    break;
            }
            return Core::Unknown;
        }

        Faders:: Faders(const Banks &banks) noexcept :
        reac(banks),
        prod(banks)
        {
        }

        Faders:: ~Faders() noexcept
        {
        }

        void Faders:: free() noexcept { reac.free(); prod.free(); }

        std::ostream & operator<<(std::ostream &os, const Faders &F)
        {
            os << "{reac:" << F.reac << "|prod:" << F.prod << "}";
            return os;
        }


        unsigned Faders:: operator()(const XReadable   &C,
                                     const Level       &L,
                                     const Components  &E,
                                     const AddressBook &conserved)
        {
            free();
            try
            {
                unsigned flag = BALANCED;
                if(reac(C,L,E.reac,conserved)) flag |= BAD_REAC;
                if(prod(C,L,E.prod,conserved)) flag |= BAD_PROD;
                return flag;
            }
            catch(...)
            {
                free();
                throw;
            }
        }
    }

}


