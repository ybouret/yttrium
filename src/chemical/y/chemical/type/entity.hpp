//! \file

#ifndef Y_Chemical_Entity_Included
#define Y_Chemical_Entity_Included 1

#include "y/chemical/type/defs.hpp"
#include "y/graphviz/vizible.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        enum Level
        {
            TopLevel = 0,
            SubLevel = 1,
            AuxLevel = 2
        };


        class Entity : public Object, public Counted, public GraphViz::Vizible
        {
        public:
            static const size_t Levels = AuxLevel+1;

            template <typename NAME> inline
            explicit Entity(const NAME &uid, const size_t topLevel) :
            Object(),
            Counted(),
            GraphViz::Vizible(),
            name(uid),
            indx()
            {
                initialize(topLevel);
            }

            virtual ~Entity() noexcept;
            
            const String & key() const noexcept;
            const String name;
            const size_t indx[Levels];

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Entity);
            void initialize(const size_t level) noexcept;
        };

    }
}

#endif

