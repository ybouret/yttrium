//! \file

#ifndef Y_Ink_Filter_Included
#define Y_Ink_Filter_Included 1

#include "y/ink/pixmap.hpp"
#include "y/string.hpp"

namespace Yttrium
{


    namespace Ink
    {

        namespace Crux
        {
            class Filter : public Object
            {
            public:
                template <typename NAME> inline
                explicit Filter(const NAME &id) : name(id) {}
                virtual ~Filter() noexcept; //!< cleanup

                const String name;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Filter);
            };
        }

        template <typename T>
        class Filter : public Crux::Filter
        {
        public:
            explicit Filter() noexcept {}
            virtual ~Filter() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Filter);
        };

    }

}

#endif

