
//! \file

#ifndef Y_Chemical_Actors_Included
#define Y_Chemical_Actors_Included 1

#include "y/chemical/reactive/actor.hpp"
#include "y/chemical/type/assembly.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! List of actors and matching assembly
        //
        //
        //______________________________________________________________________
        class Company : public Entity, public Actor::List 
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Company() noexcept; //!< setup emptu
            virtual ~Company() noexcept; //!< cleanup
            Company(const Company &);    //!< duplicate

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const String & key() const noexcept; //!< [Entity] name

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const Actor & recruit(Actor * const);                  //!< recruit and update name
            bool          lookFor(const Species &) const noexcept; //!< look for species
            void          xch(Company &)                 noexcept; //!< exchange all

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
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Actors() noexcept; //!< setup
            virtual ~Actors() noexcept; //!< cleanup
            Actors(const Actors &);     //!< duplicate
            Y_OSTREAM_PROTO(Actors);    //!< display name

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const Actor & operator()(const Species &);                 //!< register with coeff=1
            const Actor & operator()(const unsigned, const Species &); //!< register


        private:
            Y_DISABLE_ASSIGN(Actors);
            virtual ConstInterface & surrogate() const noexcept;
            friend class Components;
            Company company;
        };


    }

}

#endif