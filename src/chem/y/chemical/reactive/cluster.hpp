//! \file

#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1


#include "y/chemical/reactive/cluster/grouping.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        

        class Cluster :  public Proxy<const Grouping>
        {
        public:
            static const char * const  CallSign;
            typedef CxxListOf<Cluster> List;
            
            explicit Cluster(const Equilibrium &first);
            virtual ~Cluster() noexcept;



            Cluster *next;
            Cluster *prev;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
            Grouping eqs;

            virtual ConstInterface & surrogate() const noexcept;
        };
    }

}


#endif

