
//! \file


#ifndef Y_Chemical_Plexus_Equalizer_GainList_Included
#define Y_Chemical_Plexus_Equalizer_GainList_Included 1


#include "y/stream/xmlog.hpp"
#include "y/quantized.hpp"

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
            //! base class to display gain lists
            //
            //
            //__________________________________________________________________
            class GainList : public Quantized
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit GainList() noexcept; //!< setup
                virtual ~GainList() noexcept; //!< cleanup

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
                virtual void show(XMLog &, const char * const) const = 0; //!< display

            protected:

                //! helper for any compatible list
                template <typename LIST> static inline
                void Show(XMLog &xml, const char * const uuid, const LIST &self)
                {
                    if(!xml.verbose) return;
                    String title = uuid; title += Suffix;
                    xml.display(title,self);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(GainList);
                static const char Suffix[];
            };

        }

    }

}

#endif
