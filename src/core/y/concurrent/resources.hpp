
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
                template <typename DERIVED>
                explicit Resources(const ArcPtr<DERIVED> &stc) noexcept :
                contexts(CopyOf,stc)
                {
                }

                //! local shared contexts
                ArcPtr<ThreadContexts>  contexts;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Resources);
            };
        }


        //______________________________________________________________________
        //
        //
        //
        //! Resource to associate one RESOURCE per thread context
        //
        //
        //______________________________________________________________________
        template <typename RESOURCE>
        class Resources :
        public Nucleus::Resources,
        public CxxArray<RESOURCE,Nucleus::Resources::Model>
        {
        public:
            typedef CxxArray<RESOURCE,Nucleus::Resources::Model> CxxResources;
            typedef Writable<RESOURCE>                           WritableResources;
            typedef typename RESOURCE::Locus                     Locus;
            typedef typename RESOURCE::Type                      Type;

            //! setup from a derived class (Pipeline 0D/Loop [1|2]D)
            template <typename DERIVED>
            inline explicit Resources(const ArcPtr<DERIVED> &stc) :
            Nucleus::Resources(stc),
            CxxResources(CopyOf,*contexts),
            fullRange()
            {
            }

            inline virtual ~Resources() noexcept {}
            
            //! 0D setup
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

            //! 1D setup
            inline void init(const Type head, const Type tail, const Type step)
            {

            }


            //! cleanup/shutdown resources
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
