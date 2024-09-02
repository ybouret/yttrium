
//! \file

#ifndef Y_Chemical_Wardens_Included
#define Y_Chemical_Wardens_Included 1

#include "y/chemical/plexus/warden.hpp"
#include "y/chemical/plexus/clusters.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        class Wardens : public Warden::Set
        {
        public:
            
            explicit Wardens(const Clusters &);
            virtual ~Wardens() noexcept;

            void operator()(const Clusters &cls,
                            XWritable      &C,
                            XWritable      &dC,
                            const Level     L,
                            XMLog          &xml);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Wardens);
        };

    }

}

#endif
