
//! \file

#ifndef Y_Chemical_Cluster_Listing_Included
#define Y_Chemical_Cluster_Listing _Included 1

#include "y/chemical/species.hpp"
#include "y/chemical/type/meta-list.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! listing of neutral and charged species
        //
        //
        //______________________________________________________________________
        class Listing
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Listing() noexcept; //!< setup empty
            virtual ~Listing() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void   sort();                //!< sort both lists by TopLevel index
            size_t size() const noexcept; //!< sum of sizes

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const SList neutral; //!< neutral species
            const SList charged; //!< charged species


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Listing);
        };

    }

}

#endif

