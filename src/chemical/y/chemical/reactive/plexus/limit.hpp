
//! \file

#ifndef Y_Chemical_Limit_Included
#define Y_Chemical_Limit_Included 1

#include "y/chemical/reactive/actors.hpp"
#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        

        class Limit : public SRepo
        {
        public:
            //! record first species with its extent
            explicit Limit(const Species &s,
                           const xreal_t  x,
                           const SBank   &bank);
            virtual ~Limit() noexcept;
            Limit(const Limit &other);
            
            const xreal_t extent;



        private:
            Y_DISABLE_ASSIGN(Limit);
        };

       

    }

}

#endif
