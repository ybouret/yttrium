//! \file

#ifndef Y_Chemical_Equilibrium_Included
#define Y_Chemical_Equilibrium_Included 1

#include "y/chemical/reactive/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Interface for equilibrium
        //
        //
        //______________________________________________________________________
        class Equilibrium : public Entity, public Components
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char Separator = ':';                       //!< format separator
            typedef ArkPtr<String,Equilibrium>        Handle;        //!< alias
            typedef SuffixSet<String,Handle>          Set;           //!< alias
            typedef Component::Set::ConstIterator     ConstIterator; //!< alias
            static const char * const                 Colors;        //!< GraphViz color scheme

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            XReal        K(Real t);                                  //!< checked constant at a given time
            void         viz(OutputStream &, const Level)     const; //!< output node+name
            void         vizLink(OutputStream &, const Level) const; //!< make arrows
            const String vizColor(const Level)                const; //!< color=...,fontocolor=...
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! helper to display
            friend std::ostream & operator<<(std::ostream &os, const Equilibrium &eq)
            {
                os << '<' << eq.name << '>';
                return os;
            }



            //! cleanup
            virtual ~Equilibrium() noexcept;

        protected:
            //! setup
            template <typename UID> inline
            explicit Equilibrium(const UID   &uid,
                                 const size_t topLevel) :
            Entity(uid,topLevel), Components()
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibrium);
            virtual XReal getK(Real t) = 0;
        };

        typedef Small::BareLightList<const Equilibrium> EList;    //!< alias
        typedef EList::NodeType                         ENode;    //!< alias
        typedef EntitySet<Equilibrium,SubLevel>         EqSubSet; //!< alias


        

    }

}

#endif


