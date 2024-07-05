//! \file

#ifndef Y_Chemical_Components_Included
#define Y_Chemical_Components_Included 1

#include "y/chemical/reactive/component.hpp"
#include "y/chemical/reactive/actors.hpp"
#include "y/chemical/assembly.hpp"
#include "y/associative/suffix/set.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef SuffixSet<String, Component::Ptr> Manifest;

        class Components : public Entity, public Proxy<Manifest>
        {
        public:
            typedef Manifest::ConstIterator ConstIterator;

            template <typename ID>
            explicit Components(const ID &   ident,
                                const size_t level) :
            Entity(ident,level),
            Proxy<Manifest>(),
            reac(level), rfmt(),
            prod(level), pfmt(),
            db()
            {
            }

            virtual ~Components() noexcept;

            void operator()(const int      nu,
                            const Species &sp);

            const Actors   reac;
            const Assembly rfmt;
            const Actors   prod;
            const Assembly pfmt;

        private:
            Manifest db;
            virtual ConstInterface & surrogate() const noexcept;
        };
    }

}

#endif

