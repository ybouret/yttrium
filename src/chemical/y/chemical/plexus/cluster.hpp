
//! \file


#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/type/species-list.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class Cluster : public Object, public Proxy<const EList>, public Latchable
        {
        public:
            static const char * const CallSign;
            typedef CxxListOf<Cluster> List;

            explicit Cluster(Equilibrium &first);
            virtual ~Cluster() noexcept;

            void attach(Equilibrium &);
            bool accepts(const Equilibrium &) const noexcept; //!< shared species
            bool accepts(const Cluster &)     const noexcept; //!< shared species

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
            EList eqs;
        public:
            const SubSList species;

            Cluster *next;
            Cluster *prev;
        };


    }

}

#endif

