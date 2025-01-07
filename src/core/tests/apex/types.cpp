#include "y/apex/types.hpp"
#include "y/utest/run.hpp"
#include "y/calculus/base2.hpp"
#include "y/calculus/align.hpp"
#include "y/memory/allocator/archon.hpp"
#include "y/system/exception.hpp"
#include "y/object.hpp"

#include <cstring>
#include <cerrno>

namespace Yttrium
{
    namespace Apex
    {

        enum MemoryMode
        {
            StaticMemory,
            ArchonMemory
        };

        typedef uint64_t  natural_t;
        typedef int64_t   integer_t;


        template <typename T>
        struct NumberOf
        {
            static inline size_t ToHold(const size_t bytes) noexcept
            {
                return Y_ALIGN_TO(T,bytes)/sizeof(T);
            }
        };

        struct API
        {

            static const size_t   StaticBytes = 2 * sizeof(natural_t);
            static const unsigned StaticShift = iLog2<StaticBytes>:: Value;
            static const size_t   StaticWords = StaticBytes/sizeof(void*);
        };

        enum Plan
        {
            Plan1=1,
            Plan2=2,
            Plan4=4,
            Plan8=8
        };

        template <Plan PLAN>
        class Scheme
        {
        public:
            typedef typename UnsignedInt<PLAN>::Type Word;

            inline  Scheme(void * const addr, const size_t capa) noexcept
            {
            }

            inline ~Scheme() noexcept {}

            Word * const    word;
            const size_t wmax;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Scheme);
        };

        typedef Scheme<Plan1> Scheme1;
        typedef Scheme<Plan2> Scheme2;
        typedef Scheme<Plan4> Scheme4;
        typedef Scheme<Plan8> Scheme8;

        class Schemes {
        public:
            
        private:

        };



        struct Bulk
        {
            void *   entry;
            unsigned shift;
        };

#define Y_Apex_Block()  \
wksp(),                 \
bulk( *(Bulk *)wksp ),  \
capa(API::StaticBytes), \
size(0),                \
bits(0),                \
mode(StaticMemory),     \
plan(Plan1),            \
addr( (void*) wksp )

        class Block : public Object
        {
        public:
            explicit Block() noexcept :
            Y_Apex_Block()
            {
                clear();
            }

            explicit Block(size_t requested) :
            Y_Apex_Block()
            {
                clear();
                if(requested>capa)
                {
                    if(requested>Base2<size_t>::MaxPowerOfTwo)
                        throw Libc::Exception(ENOMEM,"requested too much");

                    requested    = NextPowerOfTwo(requested);
                    bulk.shift   = Base2<size_t>::Log(requested);
                    bulk.entry   = Memory::Archon::Acquire(bulk.shift);
                    Coerce(mode) = ArchonMemory;
                    Coerce(capa) = size_t(1) << bulk.shift;
                    Coerce(addr) = bulk.entry;
                }
            }




            virtual ~Block() noexcept
            {
                switch(mode)
                {
                    case StaticMemory: break;
                    case ArchonMemory:
                        memset(bulk.entry,0,capa);
                        Memory::Archon::Release(bulk.entry,bulk.shift);
                        Coerce(capa) = API::StaticBytes;
                        Coerce(mode) = StaticMemory;
                        break;
                }
                clear();
                size = 0;
                bits = 0;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Block);
            void * wksp[API::StaticWords];
            Bulk &bulk;


            void clear() noexcept {
                memset(wksp,0,sizeof(wksp));
            }

        public:
            const size_t     capa; //!< in bytes, a power of two
            size_t           size; //!< in bytes <= capa
            size_t           bits; //!< [0..bytes*8]
            const MemoryMode mode;
            const Plan       plan;
        private:
            void * const     addr;
        };




    }
}

using namespace Yttrium;
using namespace Apex;

Y_UTEST(apex_types)
{

    Y_USHOW(Apex::NumberOf<uint8_t> ::ToHold(21));
    Y_USHOW(Apex::NumberOf<uint16_t>::ToHold(21));
    Y_USHOW(Apex::NumberOf<uint32_t>::ToHold(21));
    Y_USHOW(Apex::NumberOf<uint64_t>::ToHold(21));

    Y_USHOW(Apex::API::StaticBytes);
    Y_USHOW(Apex::API::StaticShift);
    Y_USHOW(Apex::API::StaticWords);

    Y_USHOW(Memory::Archon::MinBytes);
    Y_USHOW(Memory::Archon::MinShift);

    Y_SIZEOF(Apex::Block);

    Y_SIZEOF(Apex::Scheme1);
    Y_SIZEOF(Apex::Scheme2);
    Y_SIZEOF(Apex::Scheme4);
    Y_SIZEOF(Apex::Scheme8);


    Apex::Block b0;
    Apex::Block b1(100);



}
Y_UDONE()


