
//! \file

#ifndef Y_Chemical_Wardens_Included
#define Y_Chemical_Wardens_Included 1

#include "y/chemical/plexus/warden.hpp"
#include "y/chemical/plexus/clusters.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Set of Wardens
        //
        //
        //______________________________________________________________________
        class Wardens : public Warden::Set
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            explicit Wardens(const Clusters &); //!< setup for PERSISTENT clusters
            virtual ~Wardens() noexcept;        //!< cleanup

            //__________________________________________________________________
            //
            //
            //! methods
            //
            //__________________________________________________________________

            //! apply to all registered clusters
            void operator()(XWritable      &C,
                            XWritable      &dC,
                            const Level     L,
                            XMLog          &xml);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Wardens);
        };

    }

}

#endif
