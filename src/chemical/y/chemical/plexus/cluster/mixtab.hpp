
//! \file

#ifndef Y_Chemical_MixTab_Included
#define Y_Chemical_MixTab_Included 1

#include "y/apex/api/ortho/coven/survey/integer.hpp"
#include "y/chemical/plexus/cluster/topology.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        // local definitions
        //
        //______________________________________________________________________
        typedef Yttrium::CxxArray<int,MemoryModel> iArrayType;

        
        using namespace Apex;
        using namespace Ortho;
        using namespace Coven;

        //______________________________________________________________________
        //
        //
        //
        // Mixing Weights And Stoichiometry
        //
        //
        //______________________________________________________________________
        class MixTab : public Quantized, public iArrayType
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const                  CallSign; //!< "Mixing"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! build from weights and primary topology
            explicit MixTab(const IntegerSurvey::ArrayType &arr,
                            const iMatrix                  &topo) ;

            //! cleanup
            virtual ~MixTab() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! check that at least a species vanishes
            bool isEfficientFor(const ClusterTopology &cl, AddressBook &source) const;


            //! comparison by ncof then lexicographic
            static SignType Compare(const MixTab * const lhs,
                                    const MixTab * const rhs) noexcept;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            const size_t  ncof; //!< number of coefficient = order
            iArrayType    stoi; //!< computed stoichiometru
            MixTab *      next; //!< for list
            MixTab *      prev; //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MixTab);
        };

    }

}

#endif

