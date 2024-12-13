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
            Proxy<const Conduct::List>(),
            my()
            {

                for(const Law *lp=laws->head;lp;lp=lp->next)
                {
                    const Law &law      = *lp;
                    bool       accepted = false;
                    for(Conduct *cn=my.head;cn;cn=cn->next)
                    {
                        if(cn->accepts(law))
                        {
                            cn->collect(law);
                            accepted = true;
                            // fusion
                            break;
                        }
                    }

                    if(!accepted)
                    {
                        // start new group
                        my.pushTail( new Conduct(law) );
                    }
                }



            }

            Conducts:: ConstInterface & Conducts:: surrogate() const noexcept { return my; }

        }

    }

}

