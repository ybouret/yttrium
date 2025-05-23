
#include "y/chemical/plexus/type/cursors.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        XBanks::  XBanks() :  species(), cursors() {}
        XBanks::  XBanks(const XBanks & _) noexcept : species(_.species), cursors(_.cursors) {}
        XBanks:: ~XBanks() noexcept {}
    }

}


namespace Yttrium
{
    namespace Chemical
    {

        Cursors:: ~Cursors() noexcept
        {
        }

        Cursors:: Cursors(const XBanks &_) noexcept:
        Proxy<const CrRepo>(),
        my(_.cursors),
        sb(_.species)
        {
        }

        Y_PROXY_IMPL(Cursors,my)


        Cursors:: Cursors(const Cursors &_) :
        Proxy<const CrRepo>(),
        my(_.my),
        sb(_.sb)
        {
        }

        void Cursors:: reset() noexcept
        {
            my.free();
        }

        bool Cursors:: areOK() const noexcept
        {
            if(my.size<2) return true;
            for(const CrNode *node=my.head;node;node=node->next)
            {
                if(node->next)
                {
                    const xReal lhs = ***node;
                    const xReal rhs = ***(node->next);
                    if(lhs>=rhs) return false;
                }
            }
            return true;
        }


        void Cursors:: operator()(const Species &s,
                                  const xReal   &x)
        {
            //------------------------------------------------------------------
            //
            //
            // get rid of specific cases
            //
            //
            //------------------------------------------------------------------
            switch(my.size)
            {
                case 0: atTail(s,x); return;

                case 1: {
                    Cursor &cr = **my.head;
                    switch( Sign::Of(x,*cr) )
                    {
                        case Negative: atHead(s,x); return;
                        case Positive: atTail(s,x); return;
                        case __Zero__: cr.join(s);  return;
                    }
                } return;

                default:
                    assert(my.size>=2);
                    break;
            }

            //------------------------------------------------------------------
            //
            //
            // check lower bound
            //
            //
            //------------------------------------------------------------------
            CrNode *lower = my.head;
            switch( Sign::Of(x,***lower) )
            {
                case Negative: atHead(s,x);       return;
                case __Zero__: (**lower).join(s); return;
                case Positive:
                    break;
            }

            //------------------------------------------------------------------
            //
            //
            // check upper bound
            //
            //
            //------------------------------------------------------------------
            CrNode * const upper = my.tail;
            switch( Sign::Of(x,***upper) )
            {
                case Positive: atTail(s,x);       return;
                case __Zero__: (**upper).join(s); return;
                case Negative:
                    break;
            }


            //------------------------------------------------------------------
            //
            //
            // generic case
            //
            //
            //------------------------------------------------------------------
        LOOK_UP:
            CrNode * next = lower->next;
            if(upper == next) goto INSERT_AFTER_LOWER;
            switch( Sign::Of(x,***next) )
            {
                case Negative: goto INSERT_AFTER_LOWER;  // found slot
                case __Zero__: (**next).join(s); return; // grow next
                case Positive: break;                    // next slot
            }
            lower = next;
            goto LOOK_UP;


        INSERT_AFTER_LOWER:
            assert(next == lower->next);
            assert(***lower < x);
            assert(x < ***next);

            const Cursor cr(s,x,sb);
            my.insertAfter(lower,my.proxy->produce(cr));
            
            assert(areOK());
        }

        void Cursors:: atTail(const Species &s, const xReal &x)
        {
            const Cursor cr(s,x,sb);
            my << cr;
            assert(areOK());
        }

        void Cursors:: atHead(const Species &s, const xReal &x)
        {
            const Cursor cr(s,x,sb);
            my >> cr;
            assert(areOK());
        }
    }

}
