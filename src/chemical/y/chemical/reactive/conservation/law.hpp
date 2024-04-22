//! \file

#ifndef Y_Chemical_Conservation_Law_Included
#define Y_Chemical_Conservation_Law_Included 1

#include "y/chemical/reactive/actors.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            class Law :  public Proxy<const Actors>
            {
            public:
                typedef CxxListOf<Law> List;
                explicit Law(const Readable<unsigned> &, const SpSubSet &);
                virtual ~Law() noexcept;

                friend std::ostream & operator<<(std::ostream &, const Law &);
                const String &key() const noexcept;

                Law *next;
                Law *prev;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Law);
                Actors       cast;
                const String uuid;

                virtual ConstInterface & surrogate() const noexcept;
            };

        }
    }
}

#endif
