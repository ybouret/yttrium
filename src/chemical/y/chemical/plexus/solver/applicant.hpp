
#ifndef Y_Chemical_Applicant_Included
#define Y_Chemical_Applicant_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/container/cxx/series.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Applicant for single equilibrium
        //
        //
        //______________________________________________________________________
        class Applicant
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxSeries<Applicant> Series; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            Applicant(const Equilibrium &  _eq,
                      const xreal_t        _eK,
                      const XReadable    & _cc,
                      const xreal_t        _xi) noexcept;

            //! copy
            Applicant(const Applicant &_) noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            size_t isub() const noexcept; //!< eq.indx[SubLevel]

            //! current affinity
            real_t affinity(XMul            &X,
                            const XReadable &C,
                            const Level      L) const;


            //! comparison for HeapSort
            static int Compare(const Applicant &lhs,
                               const Applicant &rhs) noexcept;


            //! display with optional list
            std::ostream & display(std::ostream   &os,
                                   const Assembly &uuid,
                                   const bool      full) const;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Equilibrium &eq; //!< equilibrium
            const xreal_t      eK; //!< eq.K
            const XReadable   &cc; //!< winning phase space
            const xreal_t      xi; //!< extent leading to cc
            const xreal_t      ax; //!< |xi|

        private:
            Y_DISABLE_ASSIGN(Applicant);
        };

        typedef Small::CoopLightList<const Applicant> AList;
        typedef AList::NodeType                       ANode;
        typedef AList::ProxyType                      ABank;

    }

}

#endif

