
//! \file

#ifndef Y_Ink_Format_Included
#define Y_Ink_Format_Included 1

#include "y/ink/image/codec.hpp"
#include "y/ptr/ark.hpp"
#include "y/jive/pattern/matcher.hpp"

namespace Yttrium
{
    namespace Ink
    {

        typedef Color::RGBA<uint8_t> RGBA;

        //______________________________________________________________________
        //
        //
        //
        //! Format interface
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
           
            //! setup name
            template <typename UID, typename RXP> inline
            explicit Format(const UID &uid, const RXP &rxp) :
            Codec(uid),
            extension(rxp)
            {}

        public:
            //! cleanup
            virtual ~Format() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            bool matches(const String &path);

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Jive::Matcher extension;



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Format);
        };
    }

}

#endif

