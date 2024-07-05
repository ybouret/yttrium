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

        typedef SuffixSet<String, Component::Ptr> Manifest; //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! Components: database of reac+prod
        //
        //
        //______________________________________________________________________
        class Components : public Indexed, public Proxy<Manifest>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Manifest::ConstIterator ConstIterator; //!< alias
            static const char * const       Mark;          //!< "<=>";

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
          
            //! setup
            template <typename ID>
            explicit Components(const ID & userName,
                                const size_t topLevel) :
            Indexed(userName,topLevel),
            Proxy<Manifest>(),
            reac(),
            prod(),
            db()
            {
            }

            //! cleanup
            virtual ~Components() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! insert and dispatch a new component and matching actor
            void operator()(const int      nu,
                            const Species &sp);

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Actors   reac; //!< reactants
            const Actors   prod; //!< products

        private:
            Manifest db;
            virtual ConstInterface & surrogate() const noexcept;
        };
    }

}

#endif

