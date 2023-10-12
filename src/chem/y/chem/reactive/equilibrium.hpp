
//! \file

#ifndef Y_Chemical_Equilibrium_Included
#define Y_Chemical_Equilibrium_Included 1

#include "y/chem/reactive/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! an equilibrium is a named set of components
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
            typedef ArkPtr<String,Equilibrium> Pointer; //!< alias
            typedef SuffixSet<String,Pointer>  DataBase; //!< alias
            
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Equilibrium() noexcept; //!< cleanuo

        protected:
            //! setup with identifier
            template <typename ID> inline
            explicit Equilibrium(const ID &id) : Entity(id), Components() {}

        public:
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            xreal K(double t); //!< check the return value of internal getK(t)

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibrium);
            virtual xreal getK(double) = 0;
        };


        //______________________________________________________________________
        //
        //
        //
        //! equilibrium with const constant
        //
        //
        //______________________________________________________________________
        class ConstEquilibrium : public Equilibrium
        {
        public:

            //! setup
            template <typename ID> inline
            explicit ConstEquilibrium(const ID &id, const xreal k) :
            Equilibrium(id), K_(k)
            {
            }

            //! cleanup
            virtual ~ConstEquilibrium() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ConstEquilibrium);
            const xreal K_;
            virtual xreal getK(double);
        };
    }

}

#endif
