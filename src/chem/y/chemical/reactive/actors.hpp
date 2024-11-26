
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
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Company() noexcept;
            virtual ~Company() noexcept;
            Company(const Company &);

            virtual const String & key() const noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const Actor & recruit(Actor * const);                  //!< recruit and update name
            bool          lookFor(const Species &) const noexcept; //!< look for species

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const String  name; //!< pre-computed

        private:
            Y_DISABLE_ASSIGN(Company);
        };

        //______________________________________________________________________
        //
        //
        //
        //! Actors = Proxy to const Company
        //
        //
        //______________________________________________________________________
        class Actors : public Proxy<const Company>
        {
        public:
            explicit Actors() noexcept;
            virtual ~Actors() noexcept;
            Actors(const Actors &);
            Y_OSTREAM_PROTO(Actors);

            const Actor & operator()(const Species &);
            const Actor & operator()(const unsigned, const Species &);
            

        private:
            Y_DISABLE_ASSIGN(Actors);
            virtual ConstInterface & surrogate() const noexcept;
            friend class Components;
            Company company;
        };


    }

}

#endif
