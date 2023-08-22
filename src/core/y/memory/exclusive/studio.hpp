
//! \file

#ifndef Y_Memory_Studio_Included
#define Y_Memory_Studio_Included 1

#include "y/singleton.hpp"
#include "y/memory/blanks.hpp"

namespace Yttrium
{

    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //! base class for exclusive studio
        //
        //______________________________________________________________________
        class Studio
        {
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected: explicit Studio() noexcept;
        public:    virtual ~Studio() noexcept;

            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char MultipleNew[];       //!< alias
            static const char MultipleDelete[];    //!< alias
            static const char PlacementNew[];      //!< alias
            static const char PlacementDelete[];   //!< alias

            //__________________________________________________________________
            //
            //
            // Helpers
            //
            //__________________________________________________________________
            static void *ThrowUnauthorized(const char *clid, const char *fnid);          //!< raise exception
            static void  AbortUnauthorized(const char *clid, const char *fnid) noexcept; //!< abort

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Studio);
        };
    }

    //__________________________________________________________________________
    //
    //
    //
    //! Studio to provide User-Level cache of ONE given type
    //
    //
    //__________________________________________________________________________
    template <typename T>
    class Studio :
    public Singleton< Studio<T> >,
    public Blanks<T>,
    public Core::Studio
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const char * const                  CallSign;   //!< for Singleton
        static const AtExit::Longevity             LifeTime;   //!< for Singleton
        static const size_t                        Starting;   //!< for Singleton
        typedef Singleton< Studio<T> >             Single;     //!< the Singleton




    private:
        inline explicit Studio() : Blanks<T>(Starting) {}
        inline virtual ~Studio() noexcept {}
        Y_DISABLE_COPY_AND_ASSIGN(Studio);
        friend class Singleton< Studio<T> >;
    };
    

}

#endif

