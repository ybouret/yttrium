
#ifndef Y_Chemical_Conservation_Warden_Included
#define Y_Chemical_Conservation_Warden_Included 1

#include "y/chemical/plexus/conservation/broken.hpp"
#include "y/chemical/plexus/cluster.hpp"
#include "y/chemical/type/ortho.hpp"
#include "y/mkl/algebra/lu.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {


            class Warden
            {
            public:
                static const char * const CallSign;
                
                explicit Warden(const Cluster  &_cluster,
                                const Canon    &_canon);

                virtual ~Warden() noexcept;

                void fix(XMLog     & xml,
                         XWritable & C0,
                         XWritable & I0,
                         const Level L0,
                         AddressBook &vanishing);

                const Cluster &  cluster;
                const Canon   &  canon;
                const size_t     n;      //!< canon.rank
                const size_t     m;      //!< canon.species->size
                BBank            bbank;
                BList            blist;
                BList            basis;
                XAdd             xadd;
                const QMetrics   metrics;
                QVCache          qvcache;
                QFamily          qfamily;
                MKL::LU<xreal_t> lu;
                XMatrix          A;   //!< matrix of selected constraint
                XMatrix          AT;  //!< transpose(A)
                XMatrix          AA;  //!< A*A'
                XArray           Xs;  //!< excess vector

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Warden);
            };
        }
    }
}

#endif

