
//! \file


#ifndef Y_Chemical_Equalizer_Included
#define Y_Chemical_Equalizer_Included 1

#include "y/chemical/plexus/equalizer/proceeding.hpp"
#include "y/chemical/plexus/clusters.hpp"



namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizer
        {
            
            class Proceedings : public Proxy< const ListOf<Proceeding> >
            {
            public:
                explicit Proceedings(const Clusters &);
                virtual ~Proceedings() noexcept;

                void session(XMLog     &  xml,
                             XWritable &  C0,
                             Summator  &  I0);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Proceedings);
                Y_PROXY_DECL();
                CxxListOf<Proceeding> proc;
                Banks                 banks;
                GBank                 gbank;
            };

        }

    }

}



#endif

