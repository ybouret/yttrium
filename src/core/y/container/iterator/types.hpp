
//! \file

#ifndef Y_Iterator_Types_Included
#define Y_Iterator_Types_Included 1

#include "y/config/starting.hpp"
#include "y/config/shallow.hpp"

namespace Yttrium
{
    namespace Iterating
    {
        //______________________________________________________________________
        //
        //
        //! Direction of travel
        //
        //______________________________________________________________________
        enum Direction
        {
            Forward,
            Reverse
        };

        typedef Int2Type<Forward> IsForward; //!< alias
        typedef Int2Type<Reverse> IsReverse; //!< alias

        //! Human Readable Direction
        const char *DirectionText(const Direction) noexcept;

        //______________________________________________________________________
        //
        //
        //! base class holding an aliases Way member
        //
        //______________________________________________________________________
        template <Direction WAY>
        class Travel
        {
        public:
            typedef Int2Type<WAY> Way_; //!< alias
            static const Way_     Way;  //!< alias


            inline explicit Travel() noexcept {} //!< setup
            inline virtual ~Travel() noexcept {} //!< cleanup

            //! return human readable direction
            inline const char     *directionText() const noexcept { return DirectionText(Way); }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Travel);
        };

        template <Direction WAY>
        const typename Travel<WAY>::Way_ Travel<WAY>::Way = {};
    }
}

#endif
