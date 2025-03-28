
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


        SignType Cursors:: Compare(const CrNode * const lhs, const CrNode * const rhs) noexcept
        {
            return Sign::Of( (**lhs).xi, (**rhs).xi );
        }

        bool Cursors:: checked() const noexcept
        {
            return my.isStrictlySortedBy(Compare);
        }



        void Cursors:: operator()(const Species &sp, const xreal_t xi)
        {
            // get rid of trivial cases
            //std::cerr << "placing " << sp.name << "@" << xi.str() << " into " << my << std::endl;
            switch(my.size)
            {
                case 0: my.pushTail( crNode(sp,xi) ); assert( checked() ); return;
                case 1:
                    switch( Sign::Of(xi, (**my.head).xi) )
                    {
                        case __Zero__: (**my.head) << sp;            assert( checked() ); return;
                        case Negative: my.pushHead( crNode(sp,xi) ); assert( checked() ); break;
                        case Positive: my.pushTail( crNode(sp,xi) ); assert( checked() ); break;
                    }
                    assert(2==my.size);
                    return;

                default:
                    break;
            }

            // generic case
            assert(my.size>=2);
            CrNode *lower = my.head;
            switch(Sign::Of(xi,(**lower).xi) )
            {
                case Negative: my.pushHead( crNode(sp,xi) ); assert( checked() ); return;
                case __Zero__: (**lower) << sp;              assert( checked() ); return;
                case Positive: break; // proceed
            }

            CrNode * const upper = my.tail;
            switch(Sign::Of(xi,(**upper).xi) )
            {
                case Negative: break; // proceed
                case __Zero__: (**upper) << sp;              assert( checked() ); return;
                case Positive: my.pushTail( crNode(sp,xi) ); assert( checked() ); return;
            }

        PROBE:
            CrNode * const probe = lower->next;
            if(probe != upper)
            {
                switch( Sign::Of(xi,(**probe).xi) )
                {
                    case Negative: goto FOUND;
                    case __Zero__: (**probe) << sp; assert( checked() ); return;
                    case Positive: break;
                }
                lower = probe;
                goto PROBE;
            }
        FOUND:
            my.insertAfter(lower, crNode(sp,xi) );
            assert( checked() );
        }

        CrNode * Cursors:: crNode(const Species &sp, const xreal_t xi)
        {
            const Cursor cr(sb,sp,xi);
            return my.proxy->produce(cr);
        }
    }

}

