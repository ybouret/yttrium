//! \file

#ifndef Y_Chemical_Conservation_Act_Included
#define Y_Chemical_Conservation_Act_Included 1

#include "y/chemical/plexus/conservation/law.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {


            //! group of dependent laws
            class Act : public Proxy<const Law::Group>
            {
            public:
                typedef CxxListOf<Act> List;

                explicit Act(const Law &first);
                virtual ~Act() noexcept;
                
                bool accepts(const Law &) const noexcept;
                bool accepts(const Act &) const noexcept;
                void collect(const Law &);
                void collect(const Act &);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Act);
                virtual ConstInterface & surrogate() const noexcept;

                Law::Group my;
                
            public:
                Act * next;
                Act * prev;
            };

        }

    }

}

#endif
