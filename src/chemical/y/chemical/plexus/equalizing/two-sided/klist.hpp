

//! \file

#ifndef Y_Chemical_Plexus_Equalizer_TwoSided_KList_Included
#define Y_Chemical_Plexus_Equalizer_TwoSided_KList_Included 1

#include "y/chemical/plexus/equalizing/two-sided/gain-list.hpp"
#include "y/chemical/reactive/equilibrium.hpp"

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
            //! list of blocked equilibri[um|a]
            //
            //
            //__________________________________________________________________
            class KList : public GainList, public ESolo
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef AutoPtr<KList> Pointer; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit KList() noexcept; //!< setup
                virtual ~KList() noexcept; //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                virtual void show(XMLog &xml, const char * const uuid) const; //!< display

            private:
                Y_DISABLE_COPY_AND_ASSIGN(KList);
            };
        }

    }

}

#endif

