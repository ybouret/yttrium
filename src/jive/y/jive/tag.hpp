#ifndef Y_Jive_Tag_Included
#define Y_Jive_Tag_Included 1


#include "y/string.hpp"
#include "y/ptr/ark.hpp"

namespace Yttrium
{
    namespace Jive
    {

        //______________________________________________________________________
        //
        //
        //! base class for Tag, self-keyed shared string
        //
        //______________________________________________________________________
        typedef ArkPtr<String,const String> Tag_;

        //______________________________________________________________________
        //
        //
        //
        //! convert any message into a shared string
        //
        //
        //______________________________________________________________________
        class Tag : public Tag_
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Tag(const char *  );       //!< new string from test
            explicit Tag(const String &);       //!< new string from string
            explicit Tag(const char);           //!< new string from char
            explicit Tag(const Tag &) noexcept; //!< shared copy
            virtual ~Tag() noexcept;            //!< cleanup

        private:
            Y_DISABLE_ASSIGN(Tag);
        };

        Y_SHALLOW_DECL(AsTag);

    }
}

#endif

