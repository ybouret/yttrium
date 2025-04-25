//! \file


#ifndef Y_Chemical_Initial_Repertory_Included
#define Y_Chemical_Initial_Repertory_Included 1

#include "y/chemical/plexus/initial/axioms.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Repertory : public Proxy<const Initial::Axioms::Set>
        {
        public:
            static const char * const CallSign;
            typedef Initial::Axioms::Set::ConstIterator ConstIterator;

            explicit Repertory();
            virtual ~Repertory() noexcept;
            Y_OSTREAM_PROTO(Repertory);
            
            void operator()(const XCode &xcode);

            template <typename NAME> inline
            const Initial::Axioms & operator[](const NAME &name) const {
                return query(name);
            }




        private:
            Y_DISABLE_COPY_AND_ASSIGN(Repertory);
            Y_PROXY_DECL();
            Initial::Axioms::Set my;
            const Initial::Axioms & query(const String &) const;
            const Initial::Axioms & query(const char * const) const;
        };


    }

}

#endif

