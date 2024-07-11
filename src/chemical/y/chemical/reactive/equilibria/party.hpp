
//! \file

#ifndef Y_Chemical_Equilibria_Party_Included
#define Y_Chemical_Equilibria_Party_Included 1

#include "y/chemical/reactive/equilibrium.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Helper to format party of [equilibria|cluster|...]
        //
        //
        //______________________________________________________________________
        class Party
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Party() noexcept;
            virtual ~Party() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void           update(const Components &) noexcept;                                     //!< update all metrics
            std::ostream & display(std::ostream &os, const Components &) const;                     //!< display aligned
            std::ostream & display(std::ostream &os, const Components &, const XReadable &K) const; //!< display aligned

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Assembly uuid; //!< metrics for names
            const Assembly reac; //!< metrics for reactant names
            const Assembly prod; //!< metrics for product names

            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Party);
        };
    }

}

#endif

