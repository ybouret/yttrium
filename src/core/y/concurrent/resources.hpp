
//! \file

#ifndef Y_Concurrent_Resources_Included
#define Y_Concurrent_Resources_Included 1

#include "y/concurrent/resource/interface.hpp"
#include "y/concurrent/thread/contexts.hpp"

#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/memory/solitary/workspace.hpp"

#include "y/ptr/arc.hpp"
#include "y/mkl/v2d.hpp"


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
            //! use shared thread contexts to setup resources
            //
            //
            //__________________________________________________________________
            class Resources
            {
            public:
                typedef Memory::Dyadic Model; //!< Memory Model


                virtual ~Resources() noexcept; //!< cleanup

            protected:
                //! setup with pointer conversion
                /**
                 DERIVED can be Loop/Mono/Crew or Pipeline/Alone/Queue
                 */
                template <typename DERIVED>
                explicit Resources(const ArcPtr<DERIVED> &stc) noexcept :
                contexts(CopyOf,stc)
                {
                }

                //! local shared contexts
                ArcPtr<ThreadContexts>  contexts;

                //! exception for bad 2D vertices
                void throwBadLocus2D() const;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Resources);
            };
        }


        //______________________________________________________________________
        //
        //
        //
        //! Resource to associate one RESOURCE per thread context
        /**
         RESOURCE should derive from Resource[0D|1D|2D]
         */
        //
        //
        //______________________________________________________________________
        template <typename RESOURCE>
        class Resources :
        public Nucleus::Resources,
        public CxxArray<RESOURCE,Nucleus::Resources::Model>
        {
        public:
            typedef CxxArray<RESOURCE,Nucleus::Resources::Model> CxxResources;      //!< alias
            typedef Writable<RESOURCE>                           WritableResources; //!< alias
            typedef typename RESOURCE::Locus                     Locus;             //!< resource Locus
            typedef typename RESOURCE::Type                      Type;              //!< resource unit type
            typedef V2D<Type>                                    Vertex;            //!< alias whenever 2D is used

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            //! setup from a derived class (Pipeline 0D/Loop [1|2]D)
            template <typename DERIVED>
            inline explicit Resources(const ArcPtr<DERIVED> &stc) :
            Nucleus::Resources(stc),
            CxxResources(CopyOf,*contexts),
            fullRange()
            {
            }

            inline virtual ~Resources() noexcept {}

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! 0D setup
            //__________________________________________________________________
            inline void init()
            {
                if(fullRange.isEmpty())
                {
                    WritableResources &r = *this;
                    const size_t       n = r.size();
                    try
                    {
                        for(size_t i=1;i<=n;++i) r[i].init();
                        const int one = 1;
                        fullRange.build(one);
                    }
                    catch(...)
                    {
                        quit();
                        throw;
                    }
                }
            }

            //__________________________________________________________________
            //
            //! 1D setup
            //__________________________________________________________________
            inline void init(const Type head, const Type tail, const Type step)
            {
                const Locus here(head,tail,step);
                if( fullRange.isEmpty() || here != *fullRange )
                {
                    WritableResources &r = *this;
                    const size_t       n = r.size();
                    try
                    {
                        for(size_t i=1;i<=n;++i) r[i].init(head,tail,step);
                        fullRange.build(here);
                    }
                    catch(...)
                    {
                        quit();
                        throw;
                    }
                }
            }

            //__________________________________________________________________
            //
            //! 2D setup
            //__________________________________________________________________
            inline void init(const Vertex lower, const Vertex upper)
            {
                // sanity check
                if(lower.x>upper.x||lower.y>upper.y) throwBadLocus2D();

                // create 2D requested full range
                const Locus here(lower,upper);
                if( fullRange.isEmpty() || here != *fullRange )
                {
                    WritableResources &r = *this;
                    const size_t       n = r.size();
                    try
                    {
                        for(size_t i=1;i<=n;++i) r[i].init(lower,upper);
                        fullRange.build(here);
                    }
                    catch(...)
                    {
                        quit();
                        throw;
                    }
                }
            }
            

            //__________________________________________________________________
            //
            //! cleanup/shutdown all resources
            //__________________________________________________________________
            inline void quit() noexcept
            {
                WritableResources &r = *this;
                for(size_t i=r.size();i>0;--i) r[i].quit();
                fullRange.erase();
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Resources);
            Memory::Workspace<Locus> fullRange;
        };


    }

}

#endif
