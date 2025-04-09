
//! \file


#ifndef Y_Chemical_Equalizer_GainList_Included
#define Y_Chemical_Equalizer_GainList_Included 1


#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizer
        {

            class GainList
            {
            public:
                virtual ~GainList() noexcept;
                virtual void show(XMLog &, const char * const) const = 0;

            protected:
                explicit GainList() noexcept;
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
