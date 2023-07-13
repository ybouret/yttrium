
//! \file

#ifndef Y_Memory_Studio_Included
#define Y_Memory_Studio_Included 1

#include "y/singleton.hpp"
#include "y/memory/blanks.hpp"

namespace Yttrium
{

    namespace Core
    {
        class Studio
        {
        protected: explicit Studio() noexcept;
        public:    virtual ~Studio() noexcept;

            static const char MultipleNew[];
            static const char MultipleDelete[];
            static const char PlacementNew[];
            static const char PlacementDelete[];

            static void *ThrowUnauthorized(const char *clid, const char *fnid);
            static void  AbortUnauthorized(const char *clid, const char *fnid) noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Studio);
        };
    }

    //__________________________________________________________________________
    //
    //
    //! Studio to provide User-Level cache of ONE given type
    //
    //__________________________________________________________________________
    template <typename T>
    class Studio : public Blanks<T>, public Core::Studio
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
        friend class Singleton<Studio,RegularSingleton>;
    };
    

}

#endif

