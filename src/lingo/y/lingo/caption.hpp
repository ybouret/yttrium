
//! \file

#ifndef Y_Lingo_Caption_Included
#define Y_Lingo_Caption_Included 1

#include "y/string.hpp"
#include "y/ptr/ark.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //! self-keyed shared string
        typedef ArkPtr<String,const String> Caption_;


        //! Caption is a shared constant string
        class Caption : public Caption_
        {
        public:
            explicit Caption(const String &);
            explicit Caption(const char * const);
            explicit Caption(const char);
            explicit Caption(const Caption &)      noexcept;
            explicit Caption(const String * const) noexcept;
            virtual ~Caption() noexcept;
            Y_OSTREAM_PROTO(Caption);

        private:
            Y_DISABLE_ASSIGN(Caption);
        };

        Y_SHALLOW_DECL(AsCaption);

    }
}

#endif

