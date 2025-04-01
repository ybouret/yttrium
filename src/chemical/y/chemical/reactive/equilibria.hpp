
//! \file


#ifndef Y_Chemical_Equilibria_Included
#define Y_Chemical_Equilibria_Included 1


#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/reactive/fragment.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        
        typedef SuffixSet<const String,Equilibrium::Pointer> EquilibriaSet; //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! Internal content for equilibria
        //
        //
        //______________________________________________________________________
        class EquilibriaType : public EquilibriaSet, public Fragment
        {
        public:
            explicit EquilibriaType();          //!< setup
            virtual ~EquilibriaType() noexcept; //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(EquilibriaType);
        };

        //______________________________________________________________________
        //
        //
        //
        //! Set of equilibria
        //
        //
        //______________________________________________________________________
        class Equilibria : public Proxy<const EquilibriaType>, public Latchable
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const             CallSign;       //!< "Equilibria"
            typedef EquilibriaType::ConstIterator ConstIterator;  //!< alias
            typedef EquilibriaType::Iterator      Iterator;       //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Equilibria();           //!< setup
            virtual ~Equilibria() noexcept;  //!< cleanup
            Y_OSTREAM_PROTO(Equilibria);     //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! insertion of a new, VALID and LATCHED equilibrium
            void operator()(const Equilibrium::Pointer &eq);

            const Equilibrium & operator[](const String &)    const; //!< get by name
            const Equilibrium & operator[](const char *const) const; //!< get by name

            size_t   nextIndex() const noexcept; //!< db.size()+1
            Iterator begin()           noexcept; //!< alias
            Iterator end()             noexcept; //!< alias

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
            Y_PROXY_DECL();
            EquilibriaType db;
        };


    }

}

#endif

