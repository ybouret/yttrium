//! \file

#ifndef Y_Data_Small_Coop_Included
#define Y_Data_Small_Coop_Included 1

#include "y/data/small/proxy/solo.hpp"
#include "y/object.hpp"
#include "y/counted.hpp"
#include "y/ptr/arc.hpp"

namespace Yttrium
{

    namespace Small
    {

        //______________________________________________________________________
        //
        //
        //
        //! Cache to be shared amond list
        //
        //
        //______________________________________________________________________
        template <typename NODE>
        class CoopCache : public Object, public Counted, public SoloCache<NODE>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit CoopCache() noexcept  : Object(), Counted(), SoloCache<NODE>() {}
            virtual ~CoopCache() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(CoopCache);
        };


        //______________________________________________________________________
        //
        //
        //
        //! reference counting proxy to CoopCache
        //
        //
        //______________________________________________________________________
        template <typename NODE>
        class CoopProxy : public ArcPtr< CoopCache<NODE> >
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CoopCache<NODE>   CacheType; //!< alias
            typedef ArcPtr<CacheType> CachePtr;  //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline explicit CoopProxy()                   noexcept : CachePtr( new CacheType() ) { } //!< setup
            inline virtual ~CoopProxy()                   noexcept { }                               //!< cleanup
            inline explicit CoopProxy(const CoopProxy &_) noexcept : CachePtr(_) { }                 //!< copy => share



        private:
            Y_DISABLE_ASSIGN(CoopProxy);

        };



    }

}

#endif

