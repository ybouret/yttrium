
//! \file

#ifndef Y_Field_Layout_Width_Included
#define Y_Field_Layout_Width_Included 1

#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{

    namespace Field
    {

        struct ZeroCount
        {
            template <typename T> static inline
            const T & Value() noexcept {
                assert( sizeof(T) <= Bytes() );
                return *static_cast<const T *>( Entry() );
            }

            static size_t       Bytes() noexcept;
            static const void  *Entry() noexcept;

        };

        //______________________________________________________________________
        //
        //
        //
        //! Layout width and shift
        //
        //
        //______________________________________________________________________
        template <typename COUNT> class LayoutScope
        {
        protected:

            //! dummy setup
            inline explicit LayoutScope() noexcept : 
            width( ZeroCount::Value<COUNT>() ),
            shift( ZeroCount::Value<COUNT>() )
            {}

            //! copy
            inline explicit LayoutScope(const LayoutScope &other) noexcept :
            width(other.width),
            shift(other.shift)
            {}

        public:
            //! cleanup
            inline virtual ~LayoutScope() noexcept {
                Y_STATIC_ZVAR( Coerce(width) );
                Y_STATIC_ZVAR( Coerce(shift) );
            }


            const COUNT width; //!< the width
            const COUNT shift; //!< shift per dimension

        private:
            Y_DISABLE_ASSIGN(LayoutScope);
        };
    }

}
#endif

