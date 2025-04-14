
//! \file


#ifndef Y_Chemical_Hauler_Included
#define Y_Chemical_Hauler_Included 1

#include "y/chemical/reactive/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizer
        {

            //__________________________________________________________________
            //
            //
            //
            //! helper to shift concentrations with at least one to zero
            //
            //
            //__________________________________________________________________
            class Hauler : public SRepo
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Hauler(const SBank &sb) noexcept; //!< setup
                explicit Hauler(const Hauler &);           //!< duplicate
                virtual ~Hauler() noexcept;                //!< cleanup

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________

                //! return selected extent
                virtual xreal_t getXi() const noexcept = 0;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! move with getXi(), a.k.a negative product cursor
                void            zforward(const Components &E,
                                         XWritable        &C,
                                         const Level       L) const noexcept;

                //! move with -getXi(), a.k.a negative reactant cursor
                void            zreverse(const Components &E,
                                         XWritable        &C,
                                         const Level       L) const noexcept;
            private:
                Y_DISABLE_ASSIGN(Hauler);

                //! set selected list of species to null concentration
                void vanish(XWritable        &C,
                            const Level       L) const noexcept;
            };

        }

    }

}

#endif

