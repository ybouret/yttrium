
//! \file

#ifndef Y_Text_Convert_Included
#define Y_Text_Convert_Included 1

#include "y/type/ints.hpp"
#include "y/string/fwd.hpp"

namespace Yttrium
{

    
    namespace ASCII
    {
        //______________________________________________________________________
        //
        //
        //! string to data
        //
        //______________________________________________________________________
        struct Convert
        {
            //__________________________________________________________________
            //
            //! convert to uint64_t, whith overflow control, hex/dec
            //__________________________________________________________________
            static uint64_t ToU64(const char *msg, size_t len, const char *ctx);
            static uint64_t ToU64(const char *msg, const char *ctx);               //!< alias
            static uint64_t ToU64(const Core::String<char> &msg, const char *ctx); //!< alias

            //__________________________________________________________________
            //
            //! signed decimal
            //__________________________________________________________________
            static int64_t  ToI64(const char *msg, size_t len, const char *ctx);
            static int64_t  ToI64(const char *msg, const char *ctx);               //!< alias
            static int64_t  ToI64(const Core::String<char> &msg, const char *ctx); //!< alias

            //__________________________________________________________________
            //
            //! any integer from text/string
            //__________________________________________________________________
            template <typename T, typename DATA> static inline
            T To(DATA &data, const char *ctx=0)
            {
                static const Int2Type< IsSigned<T>::Value > which = {};
                return To<T,DATA>(data,ctx,which);
            }

            //__________________________________________________________________
            //
            //! any real from data
            //__________________________________________________________________
            template <typename T> static
            T ToReal(const char *msg, const size_t len, const char *ctx);

            //__________________________________________________________________
            //
            //! any real from text
            //__________________________________________________________________
            template <typename T> static
            T ToReal(const char *msg, const char *ctx);

            //__________________________________________________________________
            //
            //! any real from string
            //__________________________________________________________________
            template <typename T> static
            T ToReal(const Core::String<char> &msg, const char *ctx);

        private:
            static void Check(const uint64_t, const uint64_t, const char *ctx);

            template <typename T, typename DATA> static inline
            T To(DATA &data, const char *ctx, const Int2Type<false> &)
            {
                const uint64_t u64 = ToU64(data,ctx);
                Check(u64,IntegerFor<T>::Maximum,ctx);
                return static_cast<T>(u64);
            }

            static void Check(const int64_t, const int64_t, const int64_t, const char *ctx);

            template <typename T, typename DATA> static inline
            T To(DATA &data, const char *ctx, const Int2Type<true> &)
            {
                const int64_t i64 = ToI64(data,ctx);
                Check(IntegerFor<T>::Minimum,i64,IntegerFor<T>::Maximum,ctx);
                return static_cast<T>(i64);
            }
        };
    }

}

#endif


