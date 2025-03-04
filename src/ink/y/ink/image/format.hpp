
//! \file

#ifndef Y_Ink_Format_Included
#define Y_Ink_Format_Included 1

#include "y/ink/image/codec.hpp"
#include "y/ptr/ark.hpp"
#include "y/lingo/pattern/matching.hpp"

namespace Yttrium
{
    namespace Ink
    {

        typedef Color::RGBA<uint8_t> RGBA;

        //______________________________________________________________________
        //
        //
        //
        //! Format interface: Codec + extension management
        //
        //
        //______________________________________________________________________
        class Format : public Codec
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef ArkPtr<String,Format> Handle; //!< alias


        protected:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
           
            //! setup name+extension regular expression
            template <typename UID, typename RXP> inline
            explicit Format(const UID &uid, const RXP &rxp) :
            Codec(uid),
            extension(rxp)
            {}

        public:
            //! cleanup
            virtual ~Format() noexcept;

            //! display
            Y_OSTREAM_PROTO(Format);

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! get a lower-case extension from path
            static String LowerCaseExt(const String &path);

            //! matching the extension of path
            bool matches(const String &path);

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Lingo::Matching extension; //!< regular expression matcher



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Format);
        };
    }

}

#endif

