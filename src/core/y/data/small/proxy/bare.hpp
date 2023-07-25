//! \file

#ifndef Y_Data_Small_Bare_Included
#define Y_Data_Small_Bare_Included 1

#include "y/data/small/supply.hpp"

namespace Yttrium
{

    namespace Small
    {

        //______________________________________________________________________
        //
        //
        //
        //! pseudo cache, no kept memory
        //
        //
        //______________________________________________________________________
        template <typename NODE>
        class BareCache : public Supply
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit BareCache() noexcept : Supply() {} //!< setup
            virtual ~BareCache() noexcept {}            //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            inline virtual void   release()       noexcept {}             //!< do nothing
            inline virtual size_t stowage() const noexcept { return 0; }  //!< nothing to do

            //__________________________________________________________________
            //
            //
            // helpers
            //
            //__________________________________________________________________

            //! helper to produce a new node
            inline NODE * produce(typename NODE::ParamType args)
            { return newNode<NODE>(args);   }

            //! helper to duplicate an existing node
            inline NODE * replica(const NODE *source)
            { assert(0!=source); return dupNode<NODE>(*source); } //!< helper to produce a new node

        private:
            Y_DISABLE_COPY_AND_ASSIGN(BareCache);
            virtual void *getFlat() { return zacquire(sizeof(NODE)); }
            virtual void  putFlat(void *blockAddr) noexcept { zrelease(blockAddr,sizeof(NODE)); }
        };

        //______________________________________________________________________
        //
        //
        //
        //! using a proxy for BareCache
        //
        //
        //______________________________________________________________________
        template <typename NODE>
        class BareProxy
        {
        public:
            typedef BareCache<NODE> CacheType;                                      //!< alias
            inline explicit BareProxy()                 noexcept : cache() {}       //!< setup
            inline virtual ~BareProxy()                 noexcept {}                 //!< cleanup
            inline BareProxy(const BareProxy &)         noexcept : cache() {}       //!< copy, setup cache
            inline CacheType       * operator->()       noexcept { return &cache; } //!< access
            inline const CacheType * operator->() const noexcept { return &cache; } //!< access, const

        private:
            Y_DISABLE_ASSIGN(BareProxy);
            CacheType cache;
        };


    }

}

#endif
