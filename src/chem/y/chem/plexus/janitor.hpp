
//! \file

#ifndef Y_Chemical_Janitor_Included
#define Y_Chemical_Janitor_Included 1

#include "y/chem/plexus/cluster.hpp"
 
namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Cleanup negative concentrations
        //
        //
        //______________________________________________________________________
        class Janitor
        {
        public:
            explicit Janitor();
            virtual ~Janitor() noexcept;

            void process(const Cluster   &cluster,
                         Writable<xreal> &Corg,
                         XMLog           &xml);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Janitor);
            class Code;
            Code  *code;
        };

    }

}

#endif

