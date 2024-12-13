
//! \file

#ifndef Y_Chemical_Conservation_Conduct_Included
#define Y_Chemical_Conservation_Conduct_Included 1

#include "y/chemical/plexus/conservation/law.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {

            class Conduct : public Proxy<const Law::Group>
            {
            public:
                explicit Conduct(const Law &first);
                virtual ~Conduct() noexcept;

                bool accepts(const Law &)     const noexcept;
                bool accepts(const Conduct &) const noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Conduct);
                virtual ConstInterface & surrogate() const noexcept;

                Law::Group my;
                
            public:
                Conduct * next;
                Conduct * prev;
            };

        }

    }

}

#endif
