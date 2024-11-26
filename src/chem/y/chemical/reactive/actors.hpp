
//! \file

#ifndef Y_Chemical_Actors_Included
#define Y_Chemical_Actors_Included 1

#include "y/chemical/reactive/actor.hpp"
#include "y/chemical/type/assembly.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Company : public Entity, public Actor::List, public Assembly
        {
        public:
            explicit Company() noexcept;
            virtual ~Company() noexcept;
            Company(const Company &);

            virtual const String & key() const noexcept;

            void recruit(Actor * const);
            bool lookFor(const Species &) const noexcept;


            const String name;
            
        private:
            Y_DISABLE_ASSIGN(Company);
        };

        class Actors : public Proxy<const Company>
        {
        public:
            explicit Actors() noexcept;
            virtual ~Actors() noexcept;
            Actors(const Actors &);
            Y_OSTREAM_PROTO(Actors);

            void operator()(const Species &);
            void operator()(const unsigned, const Species &);

        private:
            Y_DISABLE_ASSIGN(Actors);
            virtual ConstInterface & surrogate() const noexcept;

            Company company;
        };


    }

}

#endif
