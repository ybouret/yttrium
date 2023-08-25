//! \file

#ifndef Y_Data_Small_Proto_Linked_Included
#define Y_Data_Small_Proto_Linked_Included 1

#include "y/container/recyclable.hpp"

namespace Yttrium
{

    namespace Small
    {
        //______________________________________________________________________
        //
        //
        //
        //! Prototype of linked objects with cache
        //
        //
        //______________________________________________________________________
        template <
        class                     LINKED,
        typename                  NODE,
        template <typename> class PROXY>
        class ProtoLinked : public LINKED, public Recyclable
        {
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
        public:
            typedef PROXY<NODE> ProxyType; //!< alias
            
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            //! setup with [no|auto] proxy
            explicit ProtoLinked() : LINKED(), proxy() {}

            //! setup with proxy (manual or copy)
            explicit ProtoLinked(const ProxyType &_) noexcept : LINKED(), proxy(_) {}

        public:
            //! cleanup
            virtual ~ProtoLinked() noexcept {  free_(); }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! send items to proxy
            virtual void free() noexcept {  free_(); }

            
        protected:
            //! non-virtual release
            inline void free_() noexcept { proxy->destroy(*this); }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
        public:
            ProxyType proxy; //!< proxy to cache

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ProtoLinked);

        };
    }

}

#endif
