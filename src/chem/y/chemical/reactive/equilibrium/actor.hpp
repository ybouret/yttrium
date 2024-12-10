//! \file

#ifndef Y_Chemical_Actor_Included
#define Y_Chemical_Actor_Included 1

#include "y/chemical/species.hpp"
#include "y/chemical/type/defs.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Naming Actor/Actors
        //
        //
        //______________________________________________________________________
        enum NameForm
        {
            NameAsNormalSpecies, //!< use raw name
            NameAsConcentration  //!< use [name]
        };

        //______________________________________________________________________
        //
        //
        //
        //! Actor = POSITIVE coefficient + species
        //
        //
        //______________________________________________________________________
        class Actor : public Entity
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Chemical::Actor"
            typedef CxxListOf<Actor>  List;     //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Actor(const unsigned, const Species &); //!< setup with
            explicit Actor(const Species &);                 //!< setup with nu=1
            virtual ~Actor() noexcept;                       //!< cleanup
            Actor(const Actor &);                            //!< duplicate


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
            void          activity(XMul &xmul, const XReadable &C, const Level L)                 const; //!< sp(C,L)^nu into xmul
            void          activity(XMul &xmul, const XReadable &C, const Level L, const xReal xi) const; //!< at sp(C,L) + xi * nu
            void          moveSafely(XWritable &C, const Level L, const xReal xi)        const noexcept; //!< assuming C stays >=0
            const Actor & operator*()                                                    const noexcept; //!< helper to Actor::List display
            String        html(const NameForm)                                                    const; //!< create html label


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const unsigned nu;   //!< stoichio>0
            const Species &sp;   //!< species
            const xReal    xn;   //!< xreal(nu)
            const unsigned n1;   //!< nu-1
            Actor *        next; //!< for list
            Actor *        prev; //!< for list
            const String   name; //!< pre-computed for actors

        private:
            Y_DISABLE_ASSIGN(Actor);
        };

    }

}

#endif

