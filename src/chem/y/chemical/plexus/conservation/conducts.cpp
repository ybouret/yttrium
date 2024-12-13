#include "y/chemical/plexus/conservation/conducts.hpp"
#include "y/chemical/plexus/conservation/laws.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {

            Conducts:: ~Conducts() noexcept
            {
            }

            Conducts:: Conducts(const Laws &laws) :
            Proxy<const Act::List>(),
            my()
            {

                for(const Law *lp=laws->head;lp;lp=lp->next)
                {
                    const Law &law      = *lp;
                    bool       accepted = false;
                    for(Act *act=my.head;act;act=act->next)
                    {
                        if(act->accepts(law))
                        {
                            act->collect(law);
                            accepted = true;
                            reconnect();
                            break;
                        }
                    }

                    if(!accepted)
                    {
                        // start new group
                        my.pushTail( new Act(law) );
                    }
                }
                
            }

            Conducts:: ConstInterface & Conducts:: surrogate() const noexcept { return my; }

            void Conducts:: reconnect()
            {
                Act::List store;
                while(my.size>0)
                {
                    AutoPtr<Act> lhs = my.popHead();

                    for(Act *rhs=store.head;rhs;rhs=rhs->next)
                    {
                        if(rhs->accepts(*lhs))
                        {

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

