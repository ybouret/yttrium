

//! \file

#ifndef Y_Chemical_Equalizer_Included
#define Y_Chemical_Equalizer_Included 1

#include "y/chemical/plexus/equalizer/altered.hpp"
#include "y/chemical/plexus/equalizer/faders.hpp"
#include "y/chemical/plexus/cluster.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Equalizer for a cluster
        //
        //
        //______________________________________________________________________
        class Equalizer
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Chemical::Equalizer"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Equalizer(const Cluster &); //!< setup
            virtual ~Equalizer() noexcept;       //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! equalize C
            size_t run(XWritable &C, const Level L, XMLog &xml);

            //__________________________________________________________________
            //
            //
            // Memners
            //
            //__________________________________________________________________
            const Cluster &        rcl;      //!< persistent cluster
            const size_t           neq;      //!< rcl.size
            const size_t           nsp;      //!< rcl.species.size
            XAdd                   xadd;     //!< for internal computation
            Banks                  banks;    //!< shared nodes for lists
            Boundary               best;     //!< best effort
            Faders::Array          faders;   //!< faders[neq]
            XMatrix                ceq;      //!< (neq,nsp) store equalized states
            XMatrix                ddc;      //!< (new,nsp) store equalized increments
            Altered::Series        altered;  //!< probed altered equalized states
            XSwell                 swell;    //!< helper to compute final C from multiple altered

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equalizer);

            //! probe unbalanced
            size_t unbalanced(const XReadable &C, const Level L, XMLog &xml);

            //! check if best effort is possible from probed boundaries
            bool hasBestEffort(const Boundary   &limiting, const Boundaries &required);

            //! produce best altered state if best effort was possible
            void addBestAlter(const Equilibrium &eq,
                              XWritable         &cc,
                              XWritable         &dc);

            //! helper to display states
            void displayAltered(XMLog &xml, const char * const title) const;

            //! keep top level and subsequent all detached states
            void prune() noexcept;

            //! update with multiple detached altered
            void updateWithSwell(XWritable &C, const Level L, XMLog &xml);


        };

    }

}

#endif

