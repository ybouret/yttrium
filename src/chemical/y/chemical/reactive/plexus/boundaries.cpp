
#include "y/chemical/reactive/plexus/boundaries.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Boundaries:: ~Boundaries() noexcept
        {
        }

        Boundaries:: Boundaries(const BBank &bbank,
                                const SBank &sbank) noexcept :
        Proxy<const BList>(),
        impl(bbank),
        repo(sbank)
        {
        }

        Boundaries:: Boundaries(const Boundaries &other) :
        Proxy<const BList>(),
        impl(other.impl),
        repo(other.repo)
        {
        }
        
        
        bool Boundaries:: contains(const Species &s) const noexcept
        {
            for(const BNode *node=impl.head;node;node=node->next)
            {
                const Boundary &l = **node;
                if(l->has(s)) return true;
            }
            return false;
        }

        bool Boundaries:: validate() const noexcept
        {
            if(impl.size<=1) return true;

            for(const BNode *node=impl.head;node->next;node=node->next)
            {
                if( (**node).xi >= (**(node->next)).xi ) return false;
            }

            return true;
        }

        void Boundaries:: reset() noexcept
        {
            impl.free();
        }



        void Boundaries:: operator()(const Species &s,
                                     const xreal_t  x)
        {
            assert( !contains(s) );
            assert(validate());

            //------------------------------------------------------------------
            //
            // create new Limit
            //
            //------------------------------------------------------------------
            BList         &self = impl;
            const Boundary here(s,x,repo); assert(1==here->size);

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
                    Boundary &mine = **(impl.head);
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
            BNode *lower = impl.head;
            {
                Boundary &mine = **lower;
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
            BNode * const upper = impl.tail; assert(upper!=lower);
            {
                Boundary &mine = **upper;
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
                BNode    * const next = lower->next;
                Boundary &       mine = **next;
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

