
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

            //__________________________________________________________________
            //
            //
            //
            //! fix broken law(s)
            //
            //
            //__________________________________________________________________
            class Warden
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const char * const CallSign; //!< "Conservation::Warden"

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup from cluster and canon
                explicit Warden(const Cluster  &_cluster,
                                const Canon    &_canon);

                //! cleanup
                virtual ~Warden() noexcept;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! fix broken laws, append increment and record vanishing species
                void fix(XMLog     &  xml,
                         XWritable &  C0,
                         Summator  &  I0,
                         const Level  L0,
                         AddressBook &vanishing);

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Cluster &  cluster; //!< parent cluster
                const Canon   &  canon;   //!< parent canon
                const size_t     n;       //!< canon.rank
                const size_t     m;       //!< canon.species->size
                BBank            bbank;   //!< bank for lists
                BList            blist;   //!< broken laws and state
                BList            basis;   //!< basis of canon.rank
                XAdd             xadd;    //!< helper
                const QMetrics   metrics; //!< to build basis
                QVCache          qvcache; //!< to build basis
                QFamily          qfamily; //!< to build basis
                MKL::LU<xreal_t> lu;      //!< solve algebraic problem
                XMatrix          A;       //!< matrix of selected constraint
                XMatrix          AT;      //!< transpose(A)
                XMatrix          AA;      //!< A*A'
                XArray           Xs;      //!< excess vector

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Warden);
            };
        }
    }
}

#endif

