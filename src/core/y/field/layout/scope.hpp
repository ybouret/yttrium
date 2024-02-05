
//! \file

#ifndef Y_Field_Layout_Width_Included
#define Y_Field_Layout_Width_Included 1

#include "y/memory/out-of-reach.hpp"
#include "y/config/shallow.hpp"

namespace Yttrium
{

    namespace Field
    {
        Y_SHALLOW_DECL(SubLayout);

        //______________________________________________________________________
        //
        //
        //
        //! providing Zero Coord[1|2|3|4]D
        //
        //
        //______________________________________________________________________
        struct ZeroCount
        {
            //! get a pseudo zero
            template <typename T> static inline
            const T & Value() noexcept {
                assert( sizeof(T) <= Bytes() );
                return *static_cast<const T *>( Entry() );
            }

            static size_t       Bytes() noexcept; //!< inner bytes
            static const void  *Entry() noexcept; //!< inner entry

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
        public:
            typedef COUNT CountType;

        protected:
            //! zero setup
            inline explicit LayoutScope() noexcept : 
            width( ZeroCount::Value<COUNT>() ),
            shift( ZeroCount::Value<COUNT>() )
            {}

            //! copy
            inline explicit LayoutScope(const LayoutScope &other) noexcept :
            width(other.width),
            shift(other.shift)
            {}

            //! sub-scope
            template <typename SUPER> inline
            explicit LayoutScope(const SubLayout_ &, const SUPER &super) noexcept :
            width( Memory::OutOfReach::Conv<const CountType,const typename SUPER::CountType>(super.width) ),
            shift( Memory::OutOfReach::Conv<const CountType,const typename SUPER::CountType>(super.shift) )
            {
            }


        public:
            //! cleanup
            inline virtual ~LayoutScope() noexcept {
                Y_STATIC_ZVAR( Coerce(width) );
                Y_STATIC_ZVAR( Coerce(shift) );
            }

            inline size_t lastShift() const noexcept
            {
                static const size_t DIM = sizeof(COUNT)/sizeof(size_t);
                return * (Memory::OutOfReach::Cast<const size_t, const COUNT>(&shift) + (DIM-1));
            }

            const COUNT width; //!< the width
            const COUNT shift; //!< shift per dimension

        private:
            Y_DISABLE_ASSIGN(LayoutScope);
        };
    }

}
#endif

