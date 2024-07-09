
//! \file

#ifndef Y_Chemical_Equilibria_Included
#define Y_Chemical_Equilibria_Included 1

#include "y/chemical/reactive/equilibria/party.hpp"
#include "y/chemical/reactive/equilibrium/constant.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef SuffixSet<String,Equilibrium::Ptr> EqSet; //!< alias


        //______________________________________________________________________
        //
        //
        //
        //! Set of Equilibria
        //
        //
        //______________________________________________________________________
        class Equilibria : public Proxy<const EqSet>, public Party
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef EqSet::ConstIterator ConstIterator; //!< alias
            typedef EqSet::Iterator     Iterator; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Equilibria();           //!< setup
            virtual ~Equilibria() noexcept;  //!< cleanup
            Y_OSTREAM_PROTO(Equilibria);     //!< display through Party

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! append a new constant equilibrium, ready to be filled
            template <typename ID>
            Equilibrium & newConstant(const ID &name, const xreal_t K)
            {
                return append( new ConstantEquilibrium(name,db.size()+1,K) );
            }

            Iterator begin() noexcept { return db.begin(); } //!< alias
            Iterator end()   noexcept { return db.end();   } //!< alias


        protected:
            //! append a newly created equilibrium
            Equilibrium &append(Equilibrium * const);

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            EqSet db; //!< internal database of equilibria


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
            virtual ConstInterface & surrogate() const noexcept;
        };

    }

}

#endif


