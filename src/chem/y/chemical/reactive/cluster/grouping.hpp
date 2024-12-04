
//! \file

#ifndef Y_Chemical_Grouping_Included
#define Y_Chemical_Grouping_Included 1


#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/reactive/fragment.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! list of linked equilibria with format helper
        //
        //
        //______________________________________________________________________
        class Grouping : public EList, public Fragment
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Chemical::Cluster"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Grouping() noexcept; //!< setup empty
            virtual ~Grouping() noexcept; //!< cleanup


            
            //__________________________________________________________________
            //
            //
            // Method
            //
            //__________________________________________________________________
            void         collect(const Equilibrium &); //!< add and upgrade
            void         collect(Grouping          &); //!< merge and upgrade

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            SList       species;   //!< synchronized list of species
            Matrix<int> iTopology; //!< synchronized topology

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Grouping);
            void clear() noexcept; //!< no species, forget fragment
            void upgrade();        //!< update  and index

        };

    }

}

#endif

