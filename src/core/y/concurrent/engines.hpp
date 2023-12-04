
//! \file

#ifndef Y_Concurrent_Engines_Included
#define Y_Concurrent_Engines_Included 1

#include "y/concurrent/engine/interface.hpp"
#include "y/concurrent/thread/contexts.hpp"

#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/dyadic.hpp"

#include "y/ptr/arc.hpp"
#include "y/mkl/v2d.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //
        //! Agnostic Shared ThreadContexts to setup Engine(s)
        //
        //
        //______________________________________________________________________
        typedef ArcPtr<ThreadContexts> SharedThreadContexts;

        namespace Nucleus
        {
            //__________________________________________________________________
            //
            //
            //
            //! Base class to convert Derived class (Loops/Pipelines)
            //
            //
            //__________________________________________________________________
            class Engines
            {
            public:
                typedef Memory::Dyadic Model; //!< Memory Model


                virtual ~Engines() noexcept; //!< cleanup

            protected:
                //! setup with pointer conversion
                template <typename DERIVED>
                explicit Engines(const ArcPtr<DERIVED> &stc) noexcept :
                contexts(CopyOf,stc)
                {
                }

                //! local shared contexts
                SharedThreadContexts contexts;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Engines);
            };
        }


        //______________________________________________________________________
        //
        //
        //
        //! Engines to associate resources per thread context
        //
        //
        //______________________________________________________________________
        template <typename ENGINE>
        class Engines :
        public Nucleus::Engines,
        public CxxArray<ENGINE,Nucleus::Engines::Model>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxArray<ENGINE,Nucleus::Engines::Model> Propulsion; //!< alias
            typedef typename ENGINE::Type                    Type;       //!< alias
            typedef V2D<Type>                                Vertex;     //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup from a derived class (Pipeline/Loop)
            template <typename DERIVED>
            inline explicit Engines(const ArcPtr<DERIVED> &stc) :
            Nucleus::Engines(stc), Propulsion( contexts->size() )
            {
            }

            //! cleanup
            inline virtual ~Engines() noexcept {}


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! 0D api
            inline void operator()(void)
            {
                assert(this->size()==contexts->size());
                const ThreadContexts &cntx = *contexts;
                Writable<ENGINE>     &self = *this;
                const size_t          n    = self.size();
                for(size_t i=1;i<=n;++i)
                {
                    self[i].start(cntx[i]);
                }
            }

            //! 1D API
            inline void operator()(const Type head, const Type tail, const Type step)
            {
                assert(this->size()==contexts->size());
                const ThreadContexts &cntx = *contexts;
                Writable<ENGINE>     &self = *this;
                const size_t          n    = self.size();
                for(size_t i=1;i<=n;++i)
                {
                    self[i].start(cntx[i],head,tail,step);
                }
            }

            //! 2D API
            inline void operator()(const Vertex lower, const Vertex upper)
            {
                assert(this->size()==contexts->size());
                const ThreadContexts &cntx = *contexts;
                Writable<ENGINE>     &self = *this;
                const size_t          n    = self.size();
                for(size_t i=1;i<=n;++i)
                {
                    self[i].start(cntx[i],lower,upper);
                }
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Engines);
            
        };


    }

}

#endif

