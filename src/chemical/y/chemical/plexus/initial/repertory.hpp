//! \file


#ifndef Y_Chemical_Initial_Repertory_Included
#define Y_Chemical_Initial_Repertory_Included 1

#include "y/chemical/plexus/initial/axioms.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {
            class Repertory : public Proxy<const Axioms::Set>
            {
            public:
                static const char * const CallSign;
                typedef Axioms::Set::Iterator ConstIterator;

                explicit Repertory();
                virtual ~Repertory() noexcept;
                void operator()(const XCode &xcode);

                template <typename NAME> inline
                const Axioms & operator[](const NAME &name) const {
                    return query(name);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Repertory);
                Y_PROXY_DECL();
                Axioms::Set my;
                const Axioms & query(const String &) const;
                const Axioms & query(const char * const) const;
            };
        }

    }

}

#endif

