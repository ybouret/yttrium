#include "y/apex/types.hpp"
#include "y/utest/run.hpp"
#include "y/system/rtti.hpp"
#include "y/calculus/base2.hpp"
#include "y/calculus/align.hpp"
#include "y/memory/allocator/archon.hpp"

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
            static const unsigned MinMemShift = StaticShift + 1;
            static const size_t   MinMemBytes = 1 << MinMemShift;

        };

        enum Plan
        {
            Plan1,
            Plan2,
            Plan4,
            Plan8
        };

        struct InMem
        {
            void *   entry;
            unsigned shift;
        };

        class Block
        {
        public:
            explicit Block() noexcept :
            wksp(),
            capa(API::StaticBytes),
            size(0),
            bits(0),
            mode(StaticMemory),
            plan(Plan1)
            {
                memset(wksp,0,sizeof(wksp));

            }

            virtual ~Block() noexcept
            {
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Block);
            void *wksp[API::StaticWords];
        public:
            const size_t     capa; //!< in bytes
            size_t           size; //!< in bytes
            size_t           bits; //!<
            const MemoryMode mode;
            const Plan       plan;
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

    Apex::Block block;


}
Y_UDONE()


