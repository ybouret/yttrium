
//! \file

#ifndef Y_Concurrent_SIMD_Included
#define Y_Concurrent_SIMD_Included 1

#include "y/concurrent/loop/interface.hpp"
#include "y/concurrent/split/tiling.hpp"
#include "y/ostream-proto.hpp"
#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/type/nullify.hpp"

namespace Yttrium
{
    namespace Concurrent
    {


        namespace Nucleus
        {
            //! base class to handle Shared Loop
            class SIMD
            {
            public:
                typedef Memory::Dyadic Model;
                virtual ~SIMD() noexcept;

            protected:
                explicit SIMD(const SharedLoop &team) noexcept;
                explicit SIMD(Loop             *team) noexcept;

                SharedLoop loop;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(SIMD);
            };

        }


        class Resource
        {
        public:
            virtual ~Resource() noexcept {}

        protected:
            explicit Resource() noexcept {}
            virtual  void attach(const ThreadContext &) = 0;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Resource);
        };


        template <typename T>
        class Resource1D : public Resource, public ForLoop<T>
        {
        public:
            inline explicit Resource1D() noexcept : ForLoop<T>() {}
            inline virtual ~Resource1D() noexcept {}

            void setup(const ThreadContext &cntx, const T head, const T tail, const T step)
            {
                ForLoop<T> trek = Split::For(cntx,head,tail,step);
                this->swapWith(trek);
                attach(cntx);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Resource1D);
        };

        template <typename T>
        class Resource2D : public Resource
        {
        public:
            typedef typename Tiling<T>::Tile Tile;
            inline explicit Resource2D() noexcept : tile(0) {}
            inline virtual ~Resource2D() noexcept {}

            void setup(const ThreadContext &cntx, 
                       const V2D<T>        &lower,
                       const V2D<T>        &upper)
            {
                Tile *tmp = Tiling<T>::Tiles::For(cntx,lower,upper);
                tile = tmp;
                attach(cntx);
            }

            inline friend std::ostream & operator<<(std::ostream &os, const Resource2D &self)
            {
                os << self.tile;
                return os;
            }


        private:
            AutoPtr<Tile> tile;
            Y_DISABLE_COPY_AND_ASSIGN(Resource2D);
        };





        template <typename T, typename RESOURCE>
        class SIMD : public Nucleus::SIMD, public CxxArray<RESOURCE,Nucleus::SIMD::Model>
        {
        public:
            typedef CxxArray<RESOURCE,Model> Resources;

            inline explicit SIMD(const SharedLoop &team) : Nucleus::SIMD(team), Resources(loop->size()) {}
            inline explicit SIMD(Loop *            team) : Nucleus::SIMD(team), Resources(loop->size()) {}
            
            inline virtual ~SIMD() noexcept {}

            void dispatch(const T head, const T tail, const T step)
            {
                Resources  &self = *this;
                const Loop &team = *loop; assert( team.size() == self.size() );
                for(size_t i=team.size();i>0;--i)
                {
                    Resource1D<T> &resource = self[i];
                    resource.setup(team[i], head, tail, step);
                }
            }

            void dispatch(const V2D<T> lower, const V2D<T> upper)
            {
                Resources  &self = *this;
                const Loop &team = *loop; assert( team.size() == self.size() );
                for(size_t i=team.size();i>0;--i)
                {
                    Resource2D<T> &resource = self[i];
                    resource.setup(team[i],lower,upper);
                }
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMD);
        };




    }

}

#endif
