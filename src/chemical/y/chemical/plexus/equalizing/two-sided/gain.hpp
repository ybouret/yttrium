
//! \file


#ifndef Y_Chemical_Equalizer_TwoSided_Gain_Included
#define Y_Chemical_Equalizer_TwoSided_Gain_Included 1


#include "y/chemical/reactive/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizing
        {
            //__________________________________________________________________
            //
            //
            //
            //! gain per equilibria
            //
            //
            //__________________________________________________________________
            class Gain
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup
                Gain(const xreal_t      _g,
                     const Components & _E,
                     const XReadable  & _c) noexcept;

                //! cleanup
                ~Gain() noexcept;

                //! duplicate
                Gain(const Gain &_) noexcept;

                //! display
                Y_OSTREAM_PROTO(Gain);

                //______________________________________________________________
                //
                //
                //Members
                //
                //______________________________________________________________
                const xreal_t     g; //!< computed gain
                const Components &E; //!< for these components
                const XReadable  &C; //!< and (persistent) resulting concentrations

            private:
                Y_DISABLE_ASSIGN(Gain);
            };

        }

    }

}

#endif

