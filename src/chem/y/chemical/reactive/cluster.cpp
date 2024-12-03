
#include "y/chemical/reactive/cluster.hpp"
#include "y/system/exception.hpp"



namespace Yttrium
{
    namespace Chemical
    {
        Cluster:: Cluster(const Equilibrium &first) :
        Proxy<const Grouping>(),
        next(0),
        prev(0),
        eqs()
        {
            eqs.collect(first);
        }

        Cluster:: ~Cluster() noexcept
        {

        }


        Cluster::ConstInterface & Cluster:: surrogate() const noexcept
        {
            return eqs;
        }

    }

}

