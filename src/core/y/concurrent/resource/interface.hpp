
//! \file

#ifndef Y_Concurrent_Resource_Included
#define Y_Concurrent_Resource_Included 1

#include "y/concurrent/thread/context.hpp"
#include <iostream>

namespace Yttrium
{
    namespace Concurrent
    {

        namespace Nucleus
        {

            //__________________________________________________________________
            //
            //
            //
            //! Interface for Resource is a ThreadContext
            //
            //
            //__________________________________________________________________
            class Resource : public ThreadContext
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                virtual ~Resource() noexcept; //!< cleanup

            protected:
                explicit Resource(const ThreadContext &) noexcept; //!< setup


                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________

                //! activate local resources
                /**
                 called once the mapping of the resource is done, so that
                 further allocation can be made for use withing thread
                 */
                virtual void activate()          = 0; 

                //! shutdown local resource
                virtual void shutdown() noexcept = 0;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Resource);
            };
        }

        
        //______________________________________________________________________
        //
        //
        //
        //! Resource to map space into sub-parts (Mapping=Ponctual|ForLoop|Tile)
        //
        //
        //______________________________________________________________________
        template <typename MAPPING>
        class Resource : public Nucleus::Resource, public MAPPING
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef MAPPING Mapping; //!< ForLoop<>, Tile,

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
          
            //! display
            inline friend std::ostream & operator<<(std::ostream &os, const Resource &self) {
                return os << "@" << self.name << ":" << static_cast<const Mapping &>(self);
            }

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! cleanup
            inline virtual ~Resource() noexcept {}

        protected:
            //! assign resource to context
            inline explicit Resource(const ThreadContext &ctx) noexcept :
            Nucleus::Resource(ctx),
            Mapping()
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Resource);
        };



    }

}

#endif
