
//! \file

#ifndef Y_Lingo_Caption_Included
#define Y_Lingo_Caption_Included 1

#include "y/string.hpp"
#include "y/ptr/ark.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        //! self-keyed shared string
        //
        //______________________________________________________________________
        typedef ArkPtr<String,const String> Caption_;


        //______________________________________________________________________
        //
        //
        //
        //! Caption is a shared constant string
        //
        //
        //______________________________________________________________________
        class Caption : public Caption_
        {
        public:
            explicit Caption(const String &);                //!< setup from string
            explicit Caption(const char * const);            //!< setup from text
            explicit Caption(const char);                    //!< setup from uniq char
            explicit Caption(const Caption &)      noexcept; //!< safe copy
            explicit Caption(const String * const) noexcept; //!< safe assign
            virtual ~Caption()                     noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Caption);                        //!< display

        private:
            Y_DISABLE_ASSIGN(Caption);
        };

        Y_SHALLOW_DECL(AsCaption); //!< helper for Context constructor

    }
}

#endif

