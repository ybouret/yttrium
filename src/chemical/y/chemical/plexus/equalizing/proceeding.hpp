

//! \file


#ifndef Y_Chemical_Equalizer_Proceeding_Included
#define Y_Chemical_Equalizer_Proceeding_Included 1

#include "y/chemical/plexus/conservation/warden.hpp"
#include "y/chemical/plexus/equalizing/two-sided.hpp"
#include "y/chemical/plexus/equalizing/one-sided.hpp"




namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizing
        {

            //__________________________________________________________________
            //
            //
            //
            //! Proceeding for negative concentrations in one cluster
            //
            //
            //__________________________________________________________________
            class Proceeding : public Quantized
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Conservation::Warden Watcher; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup
                explicit Proceeding(const Cluster &,
                                    const Banks   &,
                                    const GBank   &);

                //! cleanup
                virtual ~Proceeding() noexcept;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! equalize cluster
                /**
                 - initialize injected
                 - check conservations  (may change injected)
                 - apply two-sided      (may change injected)
                 - apply one-sided
                 
                 \param xml for output
                 \param C0  TopLevel concentratiopns
                 */
                void fix(XMLog     &  xml,
                         XWritable &  C0);

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                Summator          injected;    //!< for cluster
                Watcher::List     watchers;    //!< fix conservations
                TwoSided::List    twoSided;    //!< fix two-sided
                OneSided          oneSided;    //!< fix one-sided
                AddressBook       vanishing;   //!< comms watchers to twoSided
                Proceeding *      next;        //!< for list
                Proceeding *      prev;        //!< for list

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Proceeding);
            };

        }

    }

}



#endif

