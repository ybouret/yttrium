//! \file

#ifndef Y_Concurrent_Engine_Included
#define Y_Concurrent_Engine_Included 1

#include "y/concurrent/thread/context.hpp"


namespace Yttrium
{
    namespace Concurrent
    {
     
        namespace Nucleus
        {
            //__________________________________________________________________
            //
            //
            //! base class and interface for a compute engine
            //
            //__________________________________________________________________
            class Engine
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                virtual     ~Engine() noexcept; //!< cleanup
            protected:
                explicit     Engine() noexcept; //!< setup

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________

                //! acquire local memory to associate to a thread context
                virtual void activate(const ThreadContext &) = 0;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Engine);
            };
        }

        //______________________________________________________________________
        //
        //
        //
        //! Base class for a ND engine
        //
        //
        //______________________________________________________________________
        template <typename MAPPING>
        class Engine : public Nucleus::Engine
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            typedef MAPPING Mapping; //!< alias

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! forward display to mapping object
            inline friend std::ostream & operator<<(std::ostream &os, const Engine &self)
            {
                return os << self.mapping;
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Mapping mapping; //!< ForLoop, Tile, ...

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! cleanup
            inline virtual ~Engine() noexcept {}

        protected:
            //! default setup, mapping is empty
            inline explicit Engine() noexcept : mapping() {}

            //! update the mapping to new value, and call active with context
            inline void initiate(const ThreadContext &cntx, Mapping &temp)
            {
                mapping.xch(temp);
                activate(cntx);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Engine);
        };



    }

}

#endif

