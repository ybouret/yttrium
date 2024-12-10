

#ifndef Y_Chemical_Fragment_Included
#define Y_Chemical_Fragment_Included 1

#include "y/chemical/reactive/equilibrium/components.hpp"
#include "y/chemical/type/assembly.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! helper to display components assembly
        //
        //
        //______________________________________________________________________
        class Fragment : public Proxy<const Assembly>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Fragment()        noexcept; //!< setup
            virtual ~Fragment()        noexcept; //!< cleanup
            Fragment(const Fragment &) noexcept; //!< duplicate

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________

            //! update using iterator on components/equilibria
            template <typename ITERATOR> inline
            void updateWith(ITERATOR i, size_t n) noexcept
            {
                forget();
                while(n-- > 0) enroll( **(i++) );
            }

            //! update using node of a light list of equilibria
            template <typename NODE> inline
            void updateWith(const NODE *node) noexcept
            {
                forget();
                for(;0!=node;node=node->next) enroll( **node );
            }



            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void enroll(const Components &);  //!< update all assemblies
            void forget()           noexcept; //!< forget all assemblies
            void trades(Fragment &) noexcept; //!< for all assemblies

            //! print name:reac<=>prod:
            std::ostream & print(std::ostream &os, const Components &) const;



        private:
            Y_DISABLE_ASSIGN(Fragment);
            virtual ConstInterface & surrogate() const noexcept;
            Assembly self; //!< for names
            Assembly reac; //!< for actors
            Assembly prod; //!< for actors
        };

    }

}

#endif

