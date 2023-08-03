//! \file

#ifndef Y_HASHING_TO_INCLUDED
#define Y_HASHING_TO_INCLUDED 1


#include "y/hashing/function.hpp"
#include "y/associative/key-variety.hpp"

namespace Yttrium
{

    namespace Hashing
    {
        //______________________________________________________________________
        //
        //! function content to integral type
        //______________________________________________________________________
        template <typename T> inline
        T To(Function &H) noexcept { T res(0); H.get(&res,sizeof(T)); return res; }

        //______________________________________________________________________
        //
        //! integral hash of a block
        //______________________________________________________________________
        template <typename T> inline
        T To(Function &H, const void *block_addr, const size_t block_size) noexcept
        { T res(0); H.block(&res,sizeof(T),block_addr,block_size); return res; }

        //______________________________________________________________________
        //
        //! integral hash of a msg
        //______________________________________________________________________
        template <typename T> inline
        T To(Function &H, const char *msg) noexcept
        { T res(0); H.block(&res,sizeof(T),msg); return res; }


        //______________________________________________________________________
        //
        //! integral hash of a buffer
        //______________________________________________________________________
        template <typename T> inline
        T To(Function &H, const Memory::ReadOnlyBuffer &buf) noexcept
        { T res(0); H.block(&res,sizeof(T),buf); return res; }


        //______________________________________________________________________
        //
        //
        //
        //! convert a hashing::function to a key hasher
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

#if 0
        //______________________________________________________________________
        //
        //
        //! key hasher prototype
        //
        //______________________________________________________________________
        template <typename FUNCTION>
        class to_hkey : public to_key<size_t,FUNCTION>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline explicit to_hkey() noexcept : to_key<size_t,FUNCTION>() {} //!< setup
            inline virtual ~to_hkey() noexcept {}                             //!< cleanup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(to_hkey);
        };
#endif

    }

}

#endif

