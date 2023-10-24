
//! \file

#ifndef Y_Chemical_Custodian_Included
#define Y_Chemical_Custodian_Included 1

#include "y/chem/plexus.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Custodian of conservation laws
        //
        //
        //______________________________________________________________________
        class Custodian
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Custodian();
            virtual ~Custodian() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void           prepare(const Plexus &, XMLog &); //!< prepare accumulator for each species conserved species
            void           startup() noexcept;               //!< cleanup all accumulators
            const SpList & collect(Writable<xreal> &Cerr);   //!< collect all errors of MODIFIED species


            //! apply the law for a given cluster
            void operator()(const Cluster   &cluster,
                            Writable<xreal> &Corg,
                            XMLog           &xml);

            //! apply the law for a prepared plexus
            void operator()(const Plexus    &plexus,
                            Writable<xreal> &Corg,
                            XMLog           &xml);

            //! mostly for debug
            void query(XMLog &xml,const Entities &spfm) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Custodian);
            class Code;
            Code *code;

        };

    }

}

#endif

