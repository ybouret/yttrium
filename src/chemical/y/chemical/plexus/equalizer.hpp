
//! \file


#ifndef Y_Chemical_Equalizer_Included
#define Y_Chemical_Equalizer_Included 1

#include "y/chemical/plexus/equalizing/proceeding.hpp"
#include "y/chemical/plexus/clusters.hpp"



namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Proceedings for each cluster
        //
        //
        //______________________________________________________________________
        class Equalizer : public Proxy< const ListOf<Equalizing::Proceeding> >
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Equalizer(const Clusters &); //!< setup
            virtual ~Equalizer() noexcept;        //!< cleanup

            //__________________________________________________________________
            //
            //
            // Metohods
            //
            //__________________________________________________________________

            //! initialize I0 and apply each proceeding
            void operator()(XMLog     &  xml,
                            XWritable &  C0,
                            Summator  &  I0);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equalizer);
            Y_PROXY_DECL();
            CxxListOf<Equalizing::Proceeding> proc;
            Equalizing::Banks                 banks;
            Equalizing::GBank                 gbank;
        };

    }

}



#endif

