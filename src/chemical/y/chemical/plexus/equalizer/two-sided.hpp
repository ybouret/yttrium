//! \file


#ifndef Y_Chemical_Equalizer_TwoSided_Included
#define Y_Chemical_Equalizer_TwoSided_Included 1


#include "y/chemical/plexus/cluster.hpp"
#include "y/chemical/plexus/equalizer/extents.hpp"
#include "y/chemical/plexus/equalizer/two-sided/glist.hpp"
#include "y/chemical/plexus/equalizer/two-sided/klist.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizer
        {
           
            //__________________________________________________________________
            //
            //
            //
            //! Handling Two Sided unbalanced equilbri[um|a]
            //
            //
            //__________________________________________________________________
            class TwoSided : public Quantized
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef CxxListOf<TwoSided> List; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup
                explicit TwoSided(const Cluster               &_cluster,
                                  const Conservation::Canon   &_canon,
                                  const Banks                 &_banks,
                                  const GBank                 &_gbank);

                virtual ~TwoSided() noexcept;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! fix C0
                void fix(XMLog             &xml,
                         XWritable         &C0,
                         Summator          &I0,
                         const Level        L0,
                         const AddressBook &vanishing);

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Cluster &             cluster; //!< persistent cluster
                const Conservation::Canon & canon;   //!< persistent canon
                Extents                     probe;   //!< probe extents
                const size_t                nrows;   //!< canon.size
                const size_t                ncols;   //!< cluster->species->size
                GBank                       gbank;   //!< shared banks
                GList::Pointer              zgain;   //!< zero-gain list
                GList::Pointer              pgain;   //!< positive gain list
                KList::Pointer              klist;   //!< blocked list
                XMatrix                     c_eqz;   //!< storage
                XAdd                        xadd;    //!< helper
                TwoSided *                  next;    //!< for list
                TwoSided *                  prev;    //!< for list

            private:
                Y_DISABLE_COPY_AND_ASSIGN(TwoSided);
            };


         
        }

    }
}


#endif
