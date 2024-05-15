
//! \file

#ifndef Y_Chemical_Warden_Included
#define Y_Chemical_Warden_Included 1

#include "y/chemical/reactive/clusters.hpp"
#include "y/container/cxx/array.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {
            //__________________________________________________________________
            //
            //
            //! Warden of conservations laws, resources for a set of clusters
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
                typedef CxxArray<XAdd,MemoryModel> XAdds; //!< alias

                //______________________________________________________________
                //
                //! Broken law description
                //______________________________________________________________
                class Broken
                {
                public:
                    //__________________________________________________________
                    //
                    // C++
                    //__________________________________________________________
                    Broken(const Law &, const xreal_t, Writable<xreal_t> &, const size_t) noexcept; //!< setu[
                    Broken(const Broken&) noexcept;                                                 //!< copy
                    ~Broken() noexcept;                                                             //!< cleanup

                    //__________________________________________________________
                    //
                    // Members
                    //__________________________________________________________
                    const Law               &law; //!< the law
                    xreal_t                  bad; //!< the deviation
                    Writable<xreal_t>       &cok; //!< concentration OK
                    const size_t             pad; //!< helper to display

                private:
                    Y_DISABLE_ASSIGN(Broken);
                };
                
                typedef Small::CoopHeavyList<Broken> BrokenList; //!< alias
                typedef BrokenList::ProxyType        BrokenRepo; //!< alias
                typedef BrokenList::NodeType         BrokenNode; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! allocate resources to process any group of any cluster
                explicit Warden(const Clusters &clusters);

                //! cleanup
                virtual ~Warden() noexcept;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! inject matter if necessary
                /**
                 \param C TopLevel initial concentration
                 \param I TopLevel injected concentration
                 \param G list of groups to process
                 \param xml for verbosity
                 */
                void operator()(XWritable   &C,
                                XWritable   &I,
                                const GList &G,
                                XMLog       &xml);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Warden);
                Matrix<xreal_t> Caux;
                BrokenRepo      repo;
                BrokenList      jail;
                XAdd            xadd;
                XAdds           xinj;
                
                void process(Writable<xreal_t>  &C,
                             Writable<xreal_t>  &I,
                             const Group        &G,
                             XMLog              &xml);
            };
        }

    }

}

#endif
