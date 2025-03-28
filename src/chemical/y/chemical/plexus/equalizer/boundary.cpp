
#include "y/chemical/plexus/equalizer/boundary.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Boundary:: Boundary(const SBank &sb) noexcept :
        SRepo(sb),
        xi(0)
        {
        }

        Boundary:: ~Boundary() noexcept {}


        std::ostream & operator<<(std::ostream &os, const Boundary &b)
        {
            if(b.size)
            {
                os << std::setw(Restartable::Width) << b.xi.str() << "@" << (const SRepo &)b;
            }
            else
                os << std::setw(Restartable::Width) << Restartable::None;
            return os;
        }


        void Boundary:: restart() noexcept { free(); xi=0; }


        void Boundary:: operator()(const Species &sp, const xreal_t xx)
        {
            if(size<=0)
            {
                xi = xx;
                (*this) << sp;
            }
            else
            {
                switch(Sign::Of(xx,xi))
                {
                    case Negative: // new winner
                        xi = xx; free(); (*this) << sp;
                        break;

                    case __Zero__: // ex-aequo
                        (*this) << sp;
                        break;

                    case Positive: // discard
                        break;
                }
            }
        }

    }

}
