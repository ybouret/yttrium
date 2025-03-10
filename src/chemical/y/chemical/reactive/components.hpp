
//! \file


#ifndef Y_Chemical_Components_Included
#define Y_Chemical_Components_Included 1


#include "y/chemical/reactive/component.hpp"
#include "y/chemical/reactive/actors.hpp"
#include "y/associative/suffix/set.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef SuffixSet<const String,const Component> ComponentsType;

        class Components :
        public Entity,
        public Proxy<const ComponentsType>,
        public Latchable
        {
        public:
            static const char * const Symbol;    //!< "<=>";
            static const char         Separator = ':'; //!< for parser

            template <typename NAME>
            explicit Components(const NAME &uuid) :
            Entity( new String(uuid) ),
            Proxy<const ComponentsType>(),
            Latchable(),
            reac(Actor::AsComponentOnly),
            prod(Actor::AsComponentOnly)
            {
            }

            
            virtual ~Components() noexcept;
            Y_OSTREAM_PROTO(Components);

            void operator()(const Role, const unsigned, const Species &);
            void operator()(const Role, const  Species &);

            const Actors reac;
            const Actors prod;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Components);
            Y_PROXY_DECL();
            ComponentsType db;
        };

    }

}

#endif

