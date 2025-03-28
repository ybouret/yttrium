
#include "y/chemical/plexus/equalizer/cursors.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Y_PROXY_IMPL(Cursors,my);

        Cursors:: Cursors(const EqzBanks &banks) noexcept :
        Proxy<const CrList>(),
        my(banks.cb),
        sb(banks.sb)
        {

        }

        Cursors:: ~Cursors() noexcept
        {

        }

        void Cursors:: restart() noexcept { my.free(); }

        void Cursors:: operator()(const Species &sp, const xreal_t xi)
        {
            switch(my.size)
            {
                case 0: my.pushTail( crNode(sp,xi) ); return;
                case 1:
                    switch( Sign::Of(xi, (**my.head).xi) )
                    {
                        case __Zero__: (**my.head) << sp;            return;
                        case Negative: my.pushHead( crNode(sp,xi) ); break;
                        case Positive: my.pushTail( crNode(sp,xi) ); break;
                    }
                    assert(2==my.size);
                    return;

                default:
                    break;
            }

            CrNode *lower = my.head;
            switch(Sign::Of(xi,(**lower).xi) )
            {
                case Negative: my.pushHead( crNode(sp,xi) ); return;
                case __Zero__: (**lower) << sp;              return;
                case Positive: break;
            }

            CrNode * const upper = my.tail;
            switch(Sign::Of(xi,(**upper).xi) )
            {
                case Negative: break;
                case __Zero__: (**upper) << sp;              return;
                case Positive: my.pushTail( crNode(sp,xi) ); return;
            }


            throw  Exception("Not Implemented");

        }
    }

}

