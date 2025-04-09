
//! \file


#ifndef Y_Chemical_Equalizer_Included
#define Y_Chemical_Equalizer_Included 1

#include "y/chemical/plexus/conservation/warden.hpp"
#include "y/chemical/plexus/equalizer/two-sided.hpp"
#include "y/chemical/plexus/equalizer/one-sided.hpp"




namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizer
        {

            class Proceeding : public Quantized
            {
            public:
                typedef Conservation::Warden Watcher;;

                explicit Proceeding(const Cluster &, const Banks &);
                virtual ~Proceeding() noexcept;

                //! equalize cluster
                void session(XMLog     &  xml,
                             XWritable &  C0,
                             Summator  &  I0);

                Watcher::List     watchers;
                TwoSided::List    twoSided;
                OneSided          oneSided;
                AddressBook       vanishing;
                Proceeding *      next;
                Proceeding *      prev;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Proceeding);
            };

        }

    }

}



#endif

