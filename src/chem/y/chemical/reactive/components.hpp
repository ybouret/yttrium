//! \file

#ifndef Y_Chemical_Components_Included
#define Y_Chemical_Components_Included 1

#include "y/chemical/reactive/component.hpp"
#include "y/chemical/reactive/actors.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        typedef SuffixSet<const String, const Component> Compendium;

        class Components : public Entity, public Counted, Proxy<const Compendium>
        {
        public:
            template <typename NAME> inline
            explicit Components(const NAME &_name) :
            Entity(), Counted(), Proxy<const Compendium>(),
            name(_name), reac(), prod(), cmdb()
            {
            }
            
            virtual ~Components() noexcept;

            virtual const String & key() const noexcept;

            void operator()(const Acting role, const unsigned nu, const Species &sp);
            void operator()(const Acting role, const Species &sp);

            std::ostream & print(std::ostream &) const;


            const String name;
            const Actors reac;
            const Actors prod;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Components);
            virtual ConstInterface & surrogate() const noexcept;
            Compendium cmdb;

            Actors & actorsPlaying(const Acting) noexcept;

        };

    }

}

#endif

