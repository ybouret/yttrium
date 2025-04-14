
//! \file


#ifndef Y_Chemical_Injected_Included
#define Y_Chemical_Injected_Included 1

#include "y/chemical/plexus/cluster.hpp"



namespace Yttrium
{
    namespace Chemical
    {
#if 0
        class Injected : public Quantized, public Summator
        {
        public:
            explicit Injected(const Cluster &);
            virtual ~Injected() noexcept;
            void     reset() noexcept;
            void     query(XWritable &dC, const Level L);

            const Cluster  &root;
            Injected * next;
            Injected * prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Injected);
        };
#endif
        
    }

}

#endif

