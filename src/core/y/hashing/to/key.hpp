//! \file

#ifndef Y_HASHING_TO_KEY_INCLUDED
#define Y_HASHING_TO_KEY_INCLUDED 1


#include "y/hashing/to.hpp"

namespace Yttrium
{

    namespace Hashing
    {
        //______________________________________________________________________
        //
        //
        //
        //! convert a Hashing::Function to a key hasher
        //
        //
        //______________________________________________________________________
        template <typename T, typename FUNCTION>
        class ToKey : public FUNCTION
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline explicit ToKey() noexcept : FUNCTION() {} //!< setup
            inline virtual ~ToKey() noexcept {}              //!< cleanup

            //__________________________________________________________________
            //
            //
            // methods
            //
            //__________________________________________________________________

            //! type dependent convertion
            template <typename U> inline
            T operator()(U &obj) noexcept
            {
                static const Int2Type< KeyVariety::Cull<U>::Kind > which = {};
                return compute<U>(obj,which);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ToKey);
            template <typename U>
            inline T compute( const U &buf, const KeyVariety::MemoryBuffer & ) noexcept
            { return To<T>(*this,buf); }

            template <typename U>
            inline T compute( const U &buf, const KeyVariety::IntegralType &) noexcept
            { return To<T>(*this,&buf,sizeof(U)); }

            template <typename U>
            inline T compute(const U &buf, const KeyVariety::LegacyString &) noexcept
            { return To<T>(*this,buf); }
        };
    }

}

#endif

