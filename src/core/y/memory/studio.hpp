
//! \file

#ifndef Y_Memory_Studio_Included
#define Y_Memory_Studio_Included 1

#include "y/singleton.hpp"
#include "y/memory/blanks.hpp"

namespace Yttrium
{

    //! Studio to provide User-Level cache of a given type
    template <typename T>
    class Studio : public Blanks<T>
    {
    public:
        static const char * const                  CallSign;   //!< for Singleton
        static const AtExit::Longevity             LifeTime;   //!< for Singleton
        static const size_t                        Starting;   //!< for Singleton
        typedef Singleton<Studio,RegularSingleton> Single;     //!< the Singleton


    private:
        inline explicit Studio() : Blanks<T>(Starting) {}
        inline virtual ~Studio() noexcept {}
        Y_DISABLE_COPY_AND_ASSIGN(Studio);
        friend Single;
    };
    

}

#endif

