
#include "y/chemical/reactive/plexus/limits.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Limits:: ~Limits() noexcept
        {
        }

        Limits:: Limits(const LimitsBank &lbank,
                        const SBank      &sbank) noexcept :
        impl(lbank),
        repo(sbank)
        {
        }

        bool Limits:: contains(const Species &s) const noexcept
        {
            for(const LimitsNode *node=impl.head;node;node=node->next)
            {
                const Limit &l = **node;
                if(l->has(s)) return true;
            }
            return false;
        }

        bool Limits:: validate() const noexcept
        {
            if(impl.size<=1) return true;

            for(const LimitsNode *node=impl.head;node->next;node=node->next)
            {
                if( (**node).xi >= (**(node->next)).xi ) return false;
            }

            return true;
        }

        void Limits:: reset() noexcept
        {
            impl.free();
        }



        void Limits:: operator()(const Species &s,
                                 const xreal_t  x)
        {
            assert( !contains(s) );
            assert(validate());

            //------------------------------------------------------------------
            //
            // create new Limit
            //
            //------------------------------------------------------------------
            LimitsType &self = impl;
            Limit       here(s,x,repo); assert(1==here->size);

            //------------------------------------------------------------------
            //
            // detect case
            //
            //------------------------------------------------------------------
            switch(impl.size)
            {
                    //----------------------------------------------------------
                    // initialize
                    //----------------------------------------------------------
                case 0: self << here; assert(validate()); return;

                    //----------------------------------------------------------
                    // only one node
                    //----------------------------------------------------------
                case 1: {
                    Limit &mine = **(impl.head);
                    switch( Sign::Of(mine.xi,here.xi) )
                    {
                        case Negative: self << here; assert(validate()); return;
                        case __Zero__: mine << s;    assert(validate()); return;
                        case Positive: self >> here; assert(validate()); return;
                    }
                } return;

                    //----------------------------------------------------------
                    // generic case
                    //----------------------------------------------------------
                default:
                    break;
            }

            assert(impl.size>=2);
            //------------------------------------------------------------------
            //
            // check against head limit
            //
            //------------------------------------------------------------------
            LimitsNode *lower = impl.head;
            {
                Limit &mine = **lower;
                switch( Sign::Of(mine.xi,here.xi) )
                {
                    case Negative:                                   break; // take next step
                    case __Zero__: mine << s;    assert(validate()); return;
                    case Positive: self >> here; assert(validate()); return;
                }
            }

            //------------------------------------------------------------------
            //
            // check against tail limit
            //
            //------------------------------------------------------------------
            LimitsNode * const upper = impl.tail; assert(upper!=lower);
            {
                Limit &mine = **upper;
                switch( Sign::Of(mine.xi,here.xi) )
                {
                    case Negative: self << here; assert(validate()); return;
                    case __Zero__: mine << s;    assert(validate()); return;
                    case Positive:                                   break; // generic
                }
            }

            //------------------------------------------------------------------
            //
            // look for insertion point
            //
            //------------------------------------------------------------------
            while(lower->next!=upper)
            {
                assert( (**lower).xi < here.xi );
                assert(  here.xi < (**upper).xi );
                LimitsNode * const next = lower->next;
                Limit &            mine = **next;
                switch( Sign::Of(mine.xi,here.xi) )
                {
                    case Negative:                                break;
                    case __Zero__: mine << s; assert(validate()); return;
                    case Positive: goto FOUND;
                }
                lower = next;
            }

        FOUND:
            assert(0!=lower);
            assert(0!=lower->next);
            assert( (**lower).xi < here.xi );
            assert( here.xi < (**(lower->next)).xi );
            impl.insertAfter(lower,impl.generate(here));
            assert(validate());
        }

    }

}

