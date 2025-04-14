
//! \file

#ifndef Y_Chemical_Plexus_Equalizer_TwoSided_GList_Included
#define Y_Chemical_Plexus_Equalizer_TwoSided_GList_Included 1

#include "y/chemical/plexus/equalizing/two-sided/gain-list.hpp"
#include "y/chemical/plexus/equalizing/two-sided/gain.hpp"
#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizing
        {
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Small ::CoopHeavyList<Gain>  GList_; //!< alias
            typedef GList_::NodeType             GNode;  //!< alias
            typedef GList_::ProxyType            GBank;  //!< alias

            //__________________________________________________________________
            //
            //
            //
            //! list of gains
            //
            //
            //__________________________________________________________________
            class GList :  public GainList, public GList_
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef AutoPtr<GList> Pointer; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit GList(const GBank &_) noexcept; //!< setup
                virtual ~GList() noexcept;               //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                virtual void show(XMLog &xml, const char * const uuid) const; //!< display

            private:
                Y_DISABLE_COPY_AND_ASSIGN(GList);
            };

        }

    }
}

#endif
    
