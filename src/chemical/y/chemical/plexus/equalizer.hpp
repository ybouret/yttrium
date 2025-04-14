
//! \file


#ifndef Y_Chemical_Equalizer_Included
#define Y_Chemical_Equalizer_Included 1

#include "y/chemical/plexus/equalizing/proceeding.hpp"
#include "y/chemical/plexus/clusters.hpp"



namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizing
        {

            //__________________________________________________________________
            //
            //
            //
            //! Proceedings for each cluster
            //
            //
            //__________________________________________________________________
            class Proceedings : public Proxy< const ListOf<Proceeding> >
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Proceedings(const Clusters &); //!< setup
                virtual ~Proceedings() noexcept;        //!< cleanup

                //______________________________________________________________
                //
                //
                // Metohods
                //
                //______________________________________________________________

                //! initialize I0 and apply each proceeding
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

