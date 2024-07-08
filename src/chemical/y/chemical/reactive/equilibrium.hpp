
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
        //! Equilibrium: components with a constant
        //
        //
        //______________________________________________________________________
        class Equilibrium : public Components
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef ArkPtr<String,Equilibrium> Ptr; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Equilibrium() noexcept; //!< cleanup

        protected:
            //! setup with name and topLevel index
            template <typename ID> inline
            explicit Equilibrium(const ID    &userName,
                                 const size_t topLevel) :
            Components(userName,topLevel),
            zero(0)
            {
            }

        public:
            Y_OSTREAM_PROTO(Equilibrium); //!< display name

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! check private getK(t)
            xreal_t K(const xreal_t t);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibrium);
            virtual xreal_t getK(const xreal_t) = 0;
            
            const xreal_t zero;
        };

    }

}

#endif

