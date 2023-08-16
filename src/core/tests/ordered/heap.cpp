
#include "y/ordered/priority-queue.hpp"
#include "y/container/interface.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/apex/natural.hpp"
#include "y/comparison.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/system/exception.hpp"

using namespace Yttrium;

namespace Yttrium
{


    //int Compiled;
    //int Flexible;


    namespace Core
    {

        template <typename T>
        class RawBuffer
        {
        public:
            Y_ARGS_DECL(T,Type);
            inline explicit RawBuffer() noexcept : addr(0), size(0), maxi(0) {}
            inline virtual ~RawBuffer() noexcept {}

            inline void free() noexcept { PriorityQueue<MutableType>::Finish(addr,size); }
            inline void make(size_t n) noexcept { free(); prepareForMaxi(n); }


        protected:
            MutableType *addr;
            size_t       size;
            const size_t maxi;

            virtual void mustAcceptNext() = 0;
            virtual void prepareForMaxi(const size_t) = 0;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(RawBuffer);
        };

        struct CompiledRawBuffer_
        {
            static void RaiseExceeded(const size_t n)
            {
                throw Specific::Exception("CompileRawBuffer","exceed maxi=%u", unsigned(n));
            }
        };

        template <size_t N, typename T>
        class CompiledRawBuffer : public RawBuffer<T>
        {
        public:
            Y_ARGS_DECL(T,Type);
            using RawBuffer<T>::maxi;
            using RawBuffer<T>::size;
            using RawBuffer<T>::addr;
            using RawBuffer<T>::free;

            explicit CompiledRawBuffer() noexcept :
            RawBuffer<T>() {
                Coerce(maxi) = N;
                addr = Memory::OutOfReach::Cast<MutableType>( Y_STATIC_ZARR(wksp) );
            }

            virtual ~CompiledRawBuffer() noexcept
            { free(); }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(CompiledRawBuffer);
            static const size_t PrivateBytes = sizeof(T) * N;
            void *wksp[ Y_WORDS_GEQ(PrivateBytes) ];

            inline virtual void mustAcceptNext() { if(size>=maxi) CompiledRawBuffer_::RaiseExceeded(maxi); }
            inline virtual void prepareForMaxi(const size_t request) { assert(0==size); if(request>maxi) CompiledRawBuffer_::RaiseExceeded(maxi); }

        };


        template <typename T, typename ALLOCATOR>
        class FlexibleRawBuffer : public RawBuffer<T>
        {
        public:
            Y_ARGS_DECL(T,Type);
            using RawBuffer<T>::maxi;
            using RawBuffer<T>::size;
            using RawBuffer<T>::addr;
            using RawBuffer<T>::free;
            explicit FlexibleRawBuffer() noexcept :  RawBuffer<T>(), wlen(0) {}
            virtual ~FlexibleRawBuffer() noexcept { release(); }
            explicit FlexibleRawBuffer(size_t n)  :  RawBuffer<T>(), wlen(0) { setup(n); }

            virtual void release() noexcept
            {
                free();
                Release(addr,Coerce(maxi),Coerce(wlen));
            }

            inline void xch(FlexibleRawBuffer &other) noexcept
            {
                CoerceSwap(addr,other.addr);
                CoerceSwap(size,other.size);
                CoerceSwap(maxi,other.maxi);
                CoerceSwap(wlen,other.wlen);
            }




        private:
            Y_DISABLE_COPY_AND_ASSIGN(FlexibleRawBuffer);
            const size_t wlen;

            static inline void Release(MutableType * & entry,
                                       size_t        & count,
                                       size_t        & bytes) noexcept
            {
                assert(Good(entry,count));
                if(entry)
                {
                    static Memory::Allocator &mgr = ALLOCATOR::Location();
                    mgr.withdraw(entry,count,bytes);
                }
            }

            void setup(const size_t request)
            {
                assert(0==addr); assert(0==size); assert(0==maxi); assert(0==wlen);
                if(request>0)
                {
                    static Memory::Allocator &mgr = ALLOCATOR::Instance();
                    addr = mgr.allocate<MutableType>( Coerce(maxi) = request, Coerce(wlen) );
                }
            }

            inline void upgradeMaxi(const size_t request)
            {
                assert(request>=maxi);
                FlexibleRawBuffer buff( request ); assert(buff.maxi>maxi);
                Memory::OutOfReach::Copy(buff.addr,addr,(buff.size=size)*sizeof(T));
                size=0;
                xch(buff);
            }

            virtual void mustAcceptNext()
            {
                if(size>=maxi)
                    upgradeMaxi( Container::NextCapacity(maxi) );
            }

            virtual void prepareForMaxi(const size_t request)
            {
                assert(0==size);
                if(request>maxi) upgradeMaxi( request );
            }
        };
    }

    template <typename T,
    typename RAW_BUFFER,
    typename COMPARATOR>
    class Heap
    {

    };



}


Y_UTEST(ordered_heap)
{

    Core::CompiledRawBuffer<10,double>             cbuf;
    Core::FlexibleRawBuffer<double,Memory::Pooled> fbuf0;
    Core::FlexibleRawBuffer<double,Memory::Pooled> fbuf(10);

    Core::PriorityQueue<double>::Insert(cbuf.addr,cbuf.size,1,Comparison::Increasing<double>);

    Core::Display(std::cerr,cbuf.addr,cbuf.size) << std::endl;

}
Y_UDONE()


