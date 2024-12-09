
//! \file

#ifndef Y_Chemical_Grouping_Included
#define Y_Chemical_Grouping_Included 1


#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/reactive/fragment.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! list of linked equilibria with format helper
        //
        //
        //______________________________________________________________________
        class Grouping : public EList, public Fragment
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Chemical::Cluster"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Grouping() noexcept; //!< setup empty
            virtual ~Grouping() noexcept; //!< cleanup


            //__________________________________________________________________
            //
            //
            // Method
            //
            //__________________________________________________________________
            void         collectPrimary(const Equilibrium &); //!< add and upgrade
            void         collectPrimary(Grouping          &); //!< merge and upgrade
            void         collectReplica(const Equilibrium &); //!< add a mixed equilibrium

            //! transfer only species from source to target
            template <typename TARGET, typename SOURCE> inline
            void transfer(TARGET &target, const Level targetLevel,
                          SOURCE &source, const Level sourceLevel) const
            {
                for(const SNode *sn=species.head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    sp(target,targetLevel) = sp(source,sourceLevel);
                }
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            SList       species;   //!< synchronized list of species
            Matrix<int> iTopology; //!< synchronized topology

        private:
            Grouping(const Grouping &);   //!< duplicate
            Y_DISABLE_ASSIGN(Grouping);
            void xch(Grouping &) noexcept; //!< exchange all
            void chk(const Equilibrium &eq) const;
        };

    }

}

#endif

