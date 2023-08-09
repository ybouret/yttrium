//! \file

#ifndef Y_Jive_Pattern_Single_Included
#define Y_Jive_Pattern_Single_Included 1

#include "y/jive/pattern.hpp"

namespace Yttrium
{
    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //
        //! Single char recognition
        //
        //
        //______________________________________________________________________
        class Single : public Pattern
        {
        public:
            static const uint32_t UUID = Y_FOURCC('S', 'N', 'G', 'L'); //!< UUID

            explicit Single(const uint8_t) noexcept; //!< setup
            virtual ~Single()              noexcept; //!< cleanup

            virtual Pattern *clone() const;          //!< clone
            

            const uint8_t code; //!< code to match

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Single);
        };
    }

}

#endif
