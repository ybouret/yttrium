
#include "y/chemical/plexus/equalizer/extent.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        const char * const Extent:: Limiting = "limiting";
        const char * const Extent:: Required = "required";


        Extent:: ~Extent() noexcept
        {
        }

        Extent:: Extent(const EqzBanks &banks) noexcept :
        Restartable(),
        limiting(banks.sb),
        required(banks)
        {
        }

        void Extent:: restart() noexcept { limiting.restart(); required.restart(); }

        std::ostream & operator<<(std::ostream &os, const Extent &ext)
        {
            os << std::endl;
            os << "\t" << Extent::Limiting << "=" << ext.limiting << std::endl;
            if(ext.required->size>0)
            {
                for(const CrNode *cn=ext.required->head;cn;cn=cn->next)
                {
                    os << "\t" << Extent::Required << "=" << **cn;
                    if(cn!=ext.required->tail) os << std::endl;
                }
            }
            else
            {
                os << "\t" << Extent::Required << "=" << std::setw(Restartable::Width) << Restartable::None;
            }
            return os;
        }


        void Extent:: operator()(const Actors &      A,
                                 const XReadable &   C,
                                 const Level         L,
                                 const AddressBook * const wanders)
        {
            restart();
            for(const Actor *a=A->head;a;a=a->next)
            {
                const Species &sp = a->sp; if(wanders && wanders->has(sp)) continue;
                const xreal_t  cc = sp(C,L);
                if(cc>=0.0)
                {
                    limiting(sp,cc/a->xn);
                }
                else
                {
                    required(sp,(-cc)/a->xn);
                }
            }
        }

    }

}
