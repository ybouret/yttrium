
//! \file

#ifndef Y_Chemical_Entity_Included
#define Y_Chemical_Entity_Included 1

#include "y/graphviz/vizible.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        enum Level
        {
            TopLevel = 0,
            SubLevel = 1,
            AuxLevel = 2,
        };

        class Entity :
        public Object,
        public Counted,
        public GraphViz::Vizible
        {
        public:
            static const size_t LEVELS = AuxLevel;

            template <typename NAME> inline
            explicit Entity(const NAME & _name,
                            const size_t _indx) :
            Object(),
            Counted(),
            GraphViz::Vizible(),
            name(_name),
            indx()
            {
                setup(_indx);
            }

            virtual ~Entity() noexcept;
            Y_OSTREAM_PROTO(Entity);

            const String & key() const noexcept;

            const String name;
            const size_t indx[LEVELS];

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Entity);
            void setup(const size_t _indx) noexcept;

        };

    }
}

#endif
