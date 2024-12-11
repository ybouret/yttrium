
#include "y/chemical/plexus/mixes.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Mixes:: ~Mixes() noexcept
        {
        }


        const char * const Mixes:: CallSign = "Chemical::Mixes";

        Mixes::ConstInterface & Mixes:: surrogate() const noexcept { return my; }

        Mixes:: Mixes(Equilibria &eqs,
                      XMLog      &xml) :
        Proxy<const Mix::List>(),
        my()
        {
            Y_XML_SECTION(xml,CallSign);



            // dispatch equilibria
            for(Equilibria::ConstIterator it=eqs->begin();it!=eqs->end();++it)
            {
                const Equilibrium &eq = **it;
                bool accepted = false;
                for(Mix *mix=my.head;mix;mix=mix->next)
                {
                    if(mix->accepts(eq))
                    {
                        mix->addPrimary(eq);
                        accepted = true;
                        break;
                    }
                }

                if(accepted)
                {
                    // check fusion
                    checkFusion();
                }
                else
                {
                    // create new Mix
                    my.pushTail( new Mix(eq) );
                }
            }


            // configure mixes
            for(Mix *mix=my.head;mix;mix=mix->next)
            {
                mix->buildConfiguration(xml,eqs);
            }

        }


        void Mixes:: checkFusion()
        {
            Mix::List store;
            while(my.size>0)
            {
                AutoPtr<Mix> mix = my.popHead();

                for(Mix *host=store.head;host;host=host->next)
                {
                    if( host->accepts(*mix) )
                    {
                        host->addPrimary(*mix);
                        mix.erase();
                        break;
                    }
                }

                if(mix.isValid())
                    store.pushTail( mix.yield() );
            }
            my.swapWith(store);
        }

        std::ostream & operator<<(std::ostream &os, const Mixes &mixes)
        {
            if(mixes->size<=0) return os << "{}";
            os << '{' << std::endl;
            for(const Mix *mix=mixes->head;mix;mix=mix->next)
            {
                os << *mix << std::endl;
            }
            os << '}';

            return os;
        }


    }

}

