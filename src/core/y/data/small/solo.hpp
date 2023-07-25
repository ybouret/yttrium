//! \file

#ifndef Y_Data_Small_Solo_Included
#define Y_Data_Small_Solo_Included 1

#include "y/data/small/inventory.hpp"

namespace Yttrium
{
    namespace Small
    {
        //______________________________________________________________________
        //
        //
        //
        //! Cache with private content
        //
        //
        //______________________________________________________________________
        template <typename NODE>
        class SoloCache : public Inventory
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit SoloCache() noexcept : Inventory(sizeof(NODE)) {} //!< setup
            virtual ~SoloCache() noexcept  {}                          //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! helper to create a new node
            NODE * produce( typename NODE::ParamType args )
            {
                return newNode<NODE>(args);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SoloCache);
        };

        //______________________________________________________________________
        //
        //
        //
        //! proxy for a SoloCache
        //
        //
        //______________________________________________________________________
        template <typename NODE>
        class SoloProxy
        {
        public:
            typedef SoloCache<NODE> CacheType;                                 //!< alias
            explicit SoloProxy()                  noexcept : cache() {}        //!< setup
            explicit SoloProxy(const SoloProxy &) noexcept : cache() {}        //!< copy, just setup cache
            virtual ~SoloProxy()                  noexcept { }                 //!< cleanup
            CacheType       * operator->()        noexcept { return &cache; }  //!< access
            const CacheType * operator->()  const noexcept { return &cache; }  //!< access, const

        private:
            Y_DISABLE_ASSIGN(SoloProxy);
            CacheType cache;

        };

    }

}

#endif

