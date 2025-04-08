
#include "y/chemical/plexus/equalizer/boundary.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizer
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

            Boundary:: Boundary(const Boundary &_) :
            SRepo(_),
            Restartable(),
            xi(_.xi)
            {
            }

            void Boundary:: xch(Boundary &_) noexcept
            {
                swapWith(_);
                Swap(xi,_.xi);
            }

            Boundary & Boundary:: operator=(const Boundary &other)
            {
                Boundary tmp(other);
                xch(tmp);
                return *this;
            }
        }
    }
}

#include "y/chemical/plexus/equalizer/cursor.hpp"
namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizer
        {
            void Boundary:: add(const Cursor &cr)
            {
                for(const SNode *sn = cr.head;sn;sn=sn->next)
                {
                    const Species &sp = **sn; assert(!has(sp));
                    (*this) << sp;
                }
            }
            
            Boundary & Boundary:: operator=(const Cursor &cr)
            {
                {
                    Boundary tmp(proxy);
                    tmp.xi = cr.xi;
                    tmp.add(cr);
                    xch(tmp);
                }
                return *this;
            }
            
        }
    }

}
