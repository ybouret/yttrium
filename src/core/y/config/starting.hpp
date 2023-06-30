//! \file

#ifndef Yttrium_Config_Starting_Included
#define Yttrium_Config_Starting_Included 1


#include "y/config/starting.h"
#include "y/config/noexcept.hpp"

#define Y_DISABLE_COPY(CLASS)   private: CLASS(const CLASS &)
#define Y_DISABLE_ASSIGN(CLASS) private: CLASS & operator=(const CLASS &)
#define Y_DISABLE_COPY_AND_ASSIGN(CLASS) \
Y_DISABLE_ASSIGN(CLASS);\
Y_DISABLE_COPY(CLASS)

namespace Yttrium
{

    template <typename T>
    inline void Swap(T &a, T &b)
    {
        T t(a);
        a = b;
        b = t;
    }

    template <typename T>
    inline T &Coerce(const T &args) noexcept
    {
        return (T&)args;
    }

    namespace Core
    {
        extern const char Unknown[];         //!< "unknown"
        extern const char Success[];         //!< "success"
        extern const char Failure[];         //!< "failure"
        extern const char Warning[];         //!< "warning"
        extern const char Message[];         //!< "message"
    }

    bool Die(const char *); //!< return true
    bool Say(const char *); //!< return false
    bool Good(const void *addr, const size_t size); //!< !(addr==NULL&&size>0) */


}

#endif

