#include "y/chemical/plexus/conservation/acts.hpp"
#include "y/chemical/plexus/conservation/laws.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {

            Acts:: ~Acts() noexcept
            {
            }



            Acts:: Acts(const Laws &laws) :
            Proxy<const Act::List>(),
            my()
            {

                //--------------------------------------------------------------
                //
                // create acts
                //
                //--------------------------------------------------------------
                for(const Law *lp=laws->head;lp;lp=lp->next)
                {
                    const Law &law      = *lp;
                    bool       accepted = false;
                    for(Act *act=my.head;act;act=act->next)
                    {
                        if(act->accepts(law))
                        {
                            accepted = true;
                            act->collect(law); // append to act
                            reconnect();       // check reconnection
                            break;
                        }
                    }

                    if(!accepted)
                    {
                        // start new group
                        my.pushTail( new Act(law) );
                    }
                }

                //--------------------------------------------------------------
                //
                // finalizing
                //
                //--------------------------------------------------------------
                for(Act *act=my.head;act;act=act->next)
                {
                    act->compile();
                }

            }

            Y_PROXY_IMPL(Acts,my)

            
            void Acts:: reconnect()
            {
                Act::List store;
                while(my.size>0)
                {
                    AutoPtr<Act> lhs = my.popHead();

                    for(Act *rhs=store.head;rhs;rhs=rhs->next)
                    {
                        if(rhs->accepts(*lhs))
                        {
                            rhs->collect(*lhs);
                            lhs.erase();
                            break;
                        }
                    }

                    if(lhs.isValid())
                        store.pushTail( lhs.yield() );
                }

                my.swapWith(store);
            }

        }


    }

}

