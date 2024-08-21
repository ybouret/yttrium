

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
        class Equalizer
        {
        public:
            static const char * const CallSign;

            explicit Equalizer(const Cluster &);
            virtual ~Equalizer() noexcept;


            size_t run(XWritable &C, const Level L, XMLog &xml);

            const Cluster &        rcl;
            const size_t           neq;
            const size_t           nsp;
            XAdd                   xadd;
            Banks                  banks;
            Boundary               best;
            Faders::Array          faders;
            XMatrix                ceq;
            XMatrix                ddc;
            Altered::Series        altered;
            XSwell                 swell;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equalizer);

            //! probe unbalanced
            size_t unbalanced(const XReadable &C, const Level L, XMLog &xml);

            //! check is best effort is possible from probed boundaries
            bool hasBestEffort(const Boundary   &limiting, const Boundaries &required);

            //! produce best altered state if best effort was possible
            void addBestAlter(const Equilibrium &eq,
                              XWritable         &cc,
                              XWritable         &dc);

            //! helper to display states
            void displayAltered(XMLog &xml, const char * const title) const;

            //! keep top level and subsequent all detached states
            void prune() noexcept;

            


        };

    }

}

#endif

