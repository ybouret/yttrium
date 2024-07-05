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
            enum Category
            {
                Nebulous,
                ReacOnly,
                ProdOnly,
                Standard
            };

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
            kind(Nebulous),
            db(),
            one(1)
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

            xreal_t massAction(const xreal_t    K,
                               XMul            &mul,
                               const XReadable &C, 
                               const Level      level) const;

            xreal_t massAction(const xreal_t    K,
                               XMul            &mul,
                               const XReadable &C0,
                               const xreal_t    xi,
                               const Level      level) const;

            

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Actors   reac; //!< reactants
            const Actors   prod; //!< products
            const Category kind; //!< precomputed category
            
            void updateKind() noexcept;
            
        private:
            Manifest db;
            virtual ConstInterface & surrogate() const noexcept;

        public:
            const xreal_t one;

            class Format
            {
            public:
                explicit Format() noexcept;
                virtual ~Format() noexcept;

                const Assembly uuid;
                const Assembly reac;
                const Assembly prod;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Format);
            };
        };
    }

}

#endif

